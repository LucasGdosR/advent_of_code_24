#include <stdio.h>
#include <string.h>

#define LINE_LENGTH 20
#define RULES_NUM 222

// Usage: compile and redirect stdout to a .dot file for visualization.
// dot usage:
// dot -Tpng graph.dot -o graph.png
void main(int argc, char const *argv[])
{
    char buffer[LINE_LENGTH];
    FILE *f = fopen("24-input-rules", "r");
    printf("digraph G {\n");
    for (int i = 0; i < RULES_NUM; i++)
    {
        fgets(buffer, LINE_LENGTH, f);
        char *i1 = strtok(buffer, " ");
        char *op = strtok(NULL, " ");
        char *i2 = strtok(NULL, " ");
        strtok(NULL, " "); // Skip arrow
        char *o = strtok(NULL, " ");
        o[3] = '\0';
        printf("\t%s -> %s [label=\"%s\"];\n\t%s -> %s [label=\"%s\"];\n", i1, o, op, i2, o, op);
    }
    fclose(f);
    printf("}");
}
