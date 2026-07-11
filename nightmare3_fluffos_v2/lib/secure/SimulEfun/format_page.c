string format_page(string *items, int columns) {
    int width, i, j, x, len;
    string ret, cell;

    if(!columns) columns = 2;
    ret = "";
    width = 75/columns;
    for(i=0, x = sizeof(items); i<x; i+=columns) {
        for(j=0; j<columns; j++) {
            if(i+j >= x) break;
            cell = items[i+j];
            if(!cell) cell = "";
            len = strlen(cell);
            if(len > width) cell = cell[0..width - 1];
            ret += arrange_string(cell, width);
        }
        ret += "\n";
    }
    return ret;
}
