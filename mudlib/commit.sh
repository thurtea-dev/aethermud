#!/bin/bash
set -e
cd ~/aethermud
git status
git add -A
git commit -m "${1:-commit}"
git push