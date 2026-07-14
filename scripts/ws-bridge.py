#!/usr/bin/python3.12
"""Telnet-to-WebSocket bridge for AetherMUD's browser client.

Browsers cannot open raw TCP telnet sockets. This process listens for
WebSocket connections on WS_PORT (default 1129) and relays bytes to the
MUD's telnet port (default 1122).

Usage:
  scripts/ws-bridge.py [--listen HOST] [--ws-port 1129] [--mud-host HOST] [--mud-port 1122]
  scripts/ws-bridge.py --daemon   # fork to background, write PID file

Environment overrides: AETHERMUD_WS_HOST, AETHERMUD_WS_PORT,
AETHERMUD_MUD_HOST, AETHERMUD_MUD_PORT, AETHERMUD_WS_PIDFILE.
"""

from __future__ import annotations

import argparse
import base64
import hashlib
import os
import select
import signal
import socket
import struct
import sys
import threading
from typing import Optional

GUID = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"
DEFAULT_WS_HOST = "0.0.0.0"
DEFAULT_WS_PORT = 1129
DEFAULT_MUD_HOST = "127.0.0.1"
DEFAULT_MUD_PORT = 1122
# Reject single WebSocket frames larger than this (bytes of payload).
MAX_FRAME = 256 * 1024
# How long to wait for the next chunk of a partial frame / send.
IO_TIMEOUT = 60.0


def log(msg: str) -> None:
    sys.stderr.write("[ws-bridge] %s\n" % msg)
    sys.stderr.flush()


def accept_http_headers(conn: socket.socket) -> dict:
    data = b""
    while b"\r\n\r\n" not in data:
        chunk = recv_some(conn, 4096)
        if chunk is None:
            raise ConnectionError("client closed during handshake")
        data += chunk
        if len(data) > 65536:
            raise ConnectionError("handshake too large")
    header_blob, _ = data.split(b"\r\n\r\n", 1)
    lines = header_blob.decode("iso-8859-1", "replace").split("\r\n")
    headers = {}
    for line in lines[1:]:
        if ":" not in line:
            continue
        key, val = line.split(":", 1)
        headers[key.strip().lower()] = val.strip()
    headers["_request_line"] = lines[0] if lines else ""
    return headers


def websocket_accept_key(key: str) -> str:
    digest = hashlib.sha1((key + GUID).encode("ascii")).digest()
    return base64.b64encode(digest).decode("ascii")


def handshake(conn: socket.socket) -> None:
    headers = accept_http_headers(conn)
    if "upgrade" not in headers.get("connection", "").lower():
        raise ConnectionError("not a websocket upgrade")
    if headers.get("upgrade", "").lower() != "websocket":
        raise ConnectionError("upgrade is not websocket")
    key = headers.get("sec-websocket-key")
    if not key:
        raise ConnectionError("missing Sec-WebSocket-Key")
    accept = websocket_accept_key(key)
    response = (
        "HTTP/1.1 101 Switching Protocols\r\n"
        "Upgrade: websocket\r\n"
        "Connection: Upgrade\r\n"
        "Sec-WebSocket-Accept: %s\r\n"
        "\r\n" % accept
    )
    send_all(conn, response.encode("ascii"))


def wait_readable(conn: socket.socket, timeout: float = IO_TIMEOUT) -> None:
    rlist, _, xlist = select.select([conn], [], [conn], timeout)
    if xlist:
        raise ConnectionError("socket error while waiting to read")
    if not rlist:
        raise ConnectionError("timeout waiting for socket data")


def wait_writable(conn: socket.socket, timeout: float = IO_TIMEOUT) -> None:
    _, wlist, xlist = select.select([], [conn], [conn], timeout)
    if xlist:
        raise ConnectionError("socket error while waiting to write")
    if not wlist:
        raise ConnectionError("timeout waiting to write")


def recv_some(conn: socket.socket, n: int) -> Optional[bytes]:
    """Read up to n bytes. None means peer closed. Retries on BlockingIOError."""
    while True:
        try:
            chunk = conn.recv(n)
        except BlockingIOError:
            wait_readable(conn)
            continue
        if not chunk:
            return None
        return chunk


def recv_exact(conn: socket.socket, n: int) -> bytes:
    """Read exactly n bytes from a possibly non-blocking socket."""
    buf = b""
    while len(buf) < n:
        chunk = recv_some(conn, n - len(buf))
        if chunk is None:
            raise ConnectionError("socket closed")
        buf += chunk
    return buf


def send_all(conn: socket.socket, data: bytes) -> None:
    """sendall that tolerates non-blocking sockets and partial writes."""
    view = memoryview(data)
    while len(view):
        try:
            sent = conn.send(view)
        except BlockingIOError:
            wait_writable(conn)
            continue
        if sent == 0:
            raise ConnectionError("socket closed on send")
        view = view[sent:]


def read_ws_frame(conn: socket.socket) -> tuple[int, bytes]:
    hdr = recv_exact(conn, 2)
    b1, b2 = hdr[0], hdr[1]
    opcode = b1 & 0x0F
    masked = (b2 & 0x80) != 0
    length = b2 & 0x7F
    if length == 126:
        length = struct.unpack("!H", recv_exact(conn, 2))[0]
    elif length == 127:
        length = struct.unpack("!Q", recv_exact(conn, 8))[0]
    if length > MAX_FRAME:
        raise ConnectionError("websocket frame too large (%d > %d)" % (length, MAX_FRAME))
    mask = recv_exact(conn, 4) if masked else b""
    payload = recv_exact(conn, length) if length else b""
    if masked:
        payload = bytes(payload[i] ^ mask[i % 4] for i in range(len(payload)))
    return opcode, payload


def send_ws_frame(conn: socket.socket, opcode: int, payload: bytes) -> None:
    if len(payload) > MAX_FRAME:
        raise ConnectionError("outbound frame too large (%d > %d)" % (len(payload), MAX_FRAME))
    fin_opcode = 0x80 | (opcode & 0x0F)
    length = len(payload)
    if length < 126:
        header = struct.pack("!BB", fin_opcode, length)
    elif length < 65536:
        header = struct.pack("!BBH", fin_opcode, 126, length)
    else:
        header = struct.pack("!BBQ", fin_opcode, 127, length)
    send_all(conn, header + payload)


def relay_session(ws: socket.socket, mud_host: str, mud_port: int) -> None:
    mud = None
    try:
        handshake(ws)
        mud = socket.create_connection((mud_host, mud_port), timeout=10)
        mud.setblocking(False)
        ws.setblocking(False)
        log("session open -> %s:%d" % (mud_host, mud_port))
        while True:
            rlist, _, xlist = select.select([ws, mud], [], [ws, mud], IO_TIMEOUT)
            if xlist:
                break
            if not rlist:
                # idle keepalive ping
                try:
                    send_ws_frame(ws, 0x9, b"ping")
                except OSError:
                    break
                continue
            if mud in rlist:
                try:
                    data = mud.recv(8192)
                except BlockingIOError:
                    data = None  # select said ready; ignore spurious wakeups
                if data is None:
                    pass
                elif data == b"":
                    break  # peer closed
                else:
                    # Chunk large MUD bursts into capped WS frames.
                    offset = 0
                    while offset < len(data):
                        send_ws_frame(ws, 0x2, data[offset:offset + MAX_FRAME])
                        offset += MAX_FRAME
            if ws in rlist:
                try:
                    opcode, payload = read_ws_frame(ws)
                except (ConnectionError, OSError) as exc:
                    log("frame read failed: %s" % exc)
                    break
                if opcode == 0x8:  # close
                    break
                if opcode == 0x9:  # ping
                    send_ws_frame(ws, 0xA, payload)
                    continue
                if opcode == 0xA:  # pong
                    continue
                if opcode in (0x1, 0x2) and payload:
                    # Browser clients often omit CR; MudOS expects CRLF or LF.
                    if b"\n" not in payload and b"\r" not in payload:
                        payload = payload + b"\n"
                    try:
                        send_all(mud, payload)
                    except (ConnectionError, OSError):
                        break
    except Exception as exc:
        log("session error: %s" % exc)
    finally:
        if mud is not None:
            try:
                mud.close()
            except OSError:
                pass
        try:
            ws.close()
        except OSError:
            pass
        log("session closed")


def serve(listen_host: str, ws_port: int, mud_host: str, mud_port: int) -> None:
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server.bind((listen_host, ws_port))
    server.listen(32)
    log("listening on %s:%d -> telnet %s:%d (max frame %d)" % (
        listen_host, ws_port, mud_host, mud_port, MAX_FRAME))

    def stop_handler(signum, frame):
        log("signal %s, exiting" % signum)
        try:
            server.close()
        except OSError:
            pass
        sys.exit(0)

    signal.signal(signal.SIGTERM, stop_handler)
    signal.signal(signal.SIGINT, stop_handler)

    while True:
        try:
            conn, addr = server.accept()
        except OSError:
            break
        log("client %s:%d" % (addr[0], addr[1]))
        t = threading.Thread(
            target=relay_session, args=(conn, mud_host, mud_port), daemon=True
        )
        t.start()


def daemonize(pidfile: str) -> None:
    if os.fork() > 0:
        sys.exit(0)
    os.setsid()
    if os.fork() > 0:
        sys.exit(0)
    sys.stdout.flush()
    sys.stderr.flush()
    with open(os.devnull, "rb") as devnull:
        os.dup2(devnull.fileno(), sys.stdin.fileno())
    # Keep stderr for logging via nohup redirection from mud.sh when used that way;
    # still write pidfile for management.
    with open(pidfile, "w", encoding="ascii") as fh:
        fh.write(str(os.getpid()) + "\n")


def main(argv: Optional[list] = None) -> int:
    parser = argparse.ArgumentParser(description="AetherMUD telnet-to-WebSocket bridge")
    parser.add_argument("--listen", default=os.environ.get("AETHERMUD_WS_HOST", DEFAULT_WS_HOST))
    parser.add_argument("--ws-port", type=int, default=int(os.environ.get("AETHERMUD_WS_PORT", DEFAULT_WS_PORT)))
    parser.add_argument("--mud-host", default=os.environ.get("AETHERMUD_MUD_HOST", DEFAULT_MUD_HOST))
    parser.add_argument("--mud-port", type=int, default=int(os.environ.get("AETHERMUD_MUD_PORT", DEFAULT_MUD_PORT)))
    parser.add_argument("--daemon", action="store_true")
    parser.add_argument(
        "--pidfile",
        default=os.environ.get(
            "AETHERMUD_WS_PIDFILE",
            os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))),
                         "nightmare3_fluffos_v2", "bin", "ws-bridge.pid"),
        ),
    )
    args = parser.parse_args(argv)

    if args.daemon:
        daemonize(args.pidfile)
        # reopen log after daemonize if stderr was closed; mud.sh uses nohup instead
    else:
        # foreground: still write pid so mud.sh status can find us when launched via nohup
        try:
            with open(args.pidfile, "w", encoding="ascii") as fh:
                fh.write(str(os.getpid()) + "\n")
        except OSError as exc:
            log("warning: could not write pidfile: %s" % exc)

    serve(args.listen, args.ws_port, args.mud_host, args.mud_port)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
