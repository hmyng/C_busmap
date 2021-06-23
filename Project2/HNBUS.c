#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jrb.h"
#include "dllist.h"
#include "fields.h"

#define INFINITIVE_VALUE 10000000

#define max 1000

typedef struct{
    JRB edges;
    JRB vertices;
} Graph;

Graph createGraph();
void dropGraph(Graph graph);
void display(Graph graph);
void addVertex(Graph graph, char *name, char *busLine);
char *getVertex(Graph graph, int id);
int hasEdge(Graph graph, char *v1, char *v2);
void addEdge(Graph graph, char *v1, char *v2);
int getNumberOfVertex(Graph graph);
int getNumberOfEdge(Graph graph);
int indegree(Graph graph, char *v, char *output[]);
int outdegree(Graph graph, char *v, char *output[]);
int getAdjacents(Graph graph, char *v);
int shortest_Path(Graph graph, char *start, char* stop, char *path[]);


void readData(Graph g, char *filename);


int main()
{
    int choice, size, total, id, i;
    char name[max], s1[max], s2[max], busLine[max], bus[max];
    
    char *filename = "busHN.txt";
    Graph graph;
    graph = createGraph();
    readData(graph, filename);

    do{
        printf("HA NOI BUS\n");
        printf("1 - So luong diem bus trong Ha Noi\n");
        printf("2 - Kiem tra hai diem bus co lien ke voi nhau khong\n");
        printf("3 - Tim kiem cac diem bus lien ke voi mot diem bus bat ky\n");
        printf("4 - Tim duong di ngan nhat giua hai diem bus\n");
        printf("5 - Cac tuyen bus di qua mot diem bus \n");
        printf("6 - Cac diem bus ma mot tuyen bus di qua\n");
        printf("0 - Thoat\n");
        printf("Lua chon cua ban: ");
        scanf("%d%*c", &choice);printf("\n");
        switch (choice)
        {

        case 1:
            size = getNumberOfVertex(graph);
            printf("Co tong cong %d diem bus trong Ha Noi.\n", size);
            
            printf("\n\n");
            break;

        case 2:
            printf("Nhap ten 2 diem bus ma ban muon kiem tra:\n");
            printf("Diem bus 1: ");
            scanf("%[^\n]", s1);
		    getchar();
            printf("Diem bus 2: ");    
		    scanf("%[^\n]", s2);
		    getchar();
            JRB node1, node2, tree1, tree2;

            node1 = jrb_find_str(graph.vertices, s1);
            node2 = jrb_find_str(graph.vertices, s2);
            if (node1 == NULL)
                printf("Khong ton tai diem  <%s>\n", s1);
            if (node2 == NULL)
                printf("Khong ton tai diem  <%s>\n", s2);

            if(hasEdge(graph, s1, s2) == 1 || hasEdge(graph, s2, s1) == 1)
            {
                printf("---> Hai diem bus tren lien ke nhau");
                tree1 = (JRB)jval_v(node1->val);
                tree2 = (JRB)jval_v(node2->val);

                printf("Tuyen bus di qua 2 diem bus <%s> va <%s> la: \n", s1, s2);
                JRB node;

                jrb_traverse(node, tree1)
                {
                    if (jrb_find_str(tree2, jval_s(node->key)) != NULL)
                        printf("%s\t", jval_s(node->key));
                }
                printf("\n");
            }
            else
                printf("Hai diem trem khong lien ke nhau");


            printf("\n\n");
            break;

        case 3:
            printf("Nhap diem bus: ");
            scanf("%[^\n]", name);
		    getchar();
            JRB node = jrb_find_str(graph.vertices, name);
            if (node == NULL)
                printf("Khong ton tai diem bus <%s>\n", name);
            else
            {
                printf("Cac diem bus lien ke <%s> la: \n", name);

                total = getAdjacents(graph, name);
                printf("\nCo %d diem bus lien ke\n", total);
            }
            printf("\n");
            break;

        case 4:
            printf("Nhap ten hai dia diem:\n");
            printf("Diem khoi dau: ");
            scanf("%[^\n]", s1);
		    getchar();
            printf("Diem can toi: ");    
		    scanf("%[^\n]", s2);
		    getchar();

            if(jrb_find_str(graph.vertices, s1) == NULL)
            {   printf("Diem bus <%s> khong ton tai!\n", s1);
                break;
            }
            if(jrb_find_str(graph.vertices, s2) == NULL)
            {
                printf("Diem bus <%s> khong ton tai!\n", s2);
                break;
            }

            char *path[2000];
            int num_path = shortest_Path(graph, s1, s2, path);
            if(num_path != INFINITIVE_VALUE)
            {
                printf("\nDuong di ngan nhat giua 2 diem bus <%s> va <%s> la:  \n", s1, s2);

                for (i = 0; i < num_path; i++)
                {
                    printf("%s ", path[i]);
                    if(i < num_path - 1)
                        printf("--> ");
                }
                printf("\nLo trinh di:\n");

                for (i = 0; i < num_path-1; i++)
                {
                    printf("<%s> --> <%s>: \n", path[i], path[i+1]);

                    JRB node1 = jrb_find_str(graph.vertices, path[i]);
                    JRB node2 = jrb_find_str(graph.vertices, path[i + 1]);

                    JRB tree1 = (JRB)jval_v(node1->val);
                    JRB tree2 = (JRB)jval_v(node2->val);

                    printf("Nhung tuyen bus di qua:\n");
                    JRB node;

                    jrb_traverse(node, tree1)
                    {
                        if(jrb_find_str(tree2, jval_s(node->key)) != NULL)
                            printf("%s\t", jval_s(node->key));
                    }
                    printf("\n");
                }
            }
            else
                printf("Khong tim duoc duong di tu <%s> den <%s>\n", s1, s2);

            printf("\n\n");
            break;

        case 5:
            printf("Nhap ten diem bus: ");
            scanf("%[^\n]", name);
		    getchar();

            JRB node_Find = jrb_find_str(graph.vertices, name);
            if(node_Find == NULL)
                printf("Khong ton tai diem bus <%s>\n", name);
            else
            {
                printf("Cac tuyen bus di qua diem <%s> la: \n", name);
                JRB tree = (JRB)jval_v(node_Find->val);
                JRB node;

                jrb_traverse(node, tree)
                    printf("%s\t", jval_s(node->key));
            }
            printf("\n\n");
            break;

        case 6:
            printf("Nhap ten tuyen bus: ");
            scanf("%[^\n]", bus);
		    getchar();
            strcpy(busLine, "[");
            strcat(busLine, bus);
            strcat(busLine, "]");

            JRB b_node, tree;
            jrb_traverse(b_node, graph.vertices)
            {
                tree = (JRB)jval_v(b_node->val);
                if(jrb_find_str(tree, busLine) != NULL)
                    printf("<%s>\n", jval_s(b_node->key));
            }
            printf("\n\n");
            break;
            case 0:
            printf("Thoat chuong trinh....\n");break;
        default:
            printf("Lua chon khong ton tai!\n");
        }
        }while(choice!=0);

    dropGraph(graph);
    printf("\n\n");
    return 0;
}


void readData(Graph g, char *filename)
{
    IS is = new_inputstruct(filename);

    if (is == NULL)
    {
        printf("Cannot read file!\n");
        return;
    }

    char *busLine;//ten tuyen

    while (get_line(is) > 0) //Doc tung dong trong file
    {
        if (strcmp(is->fields[0], "TUYEN") == 0) //kiem tra dong chua ten tuyen
        {
            busLine = (char *)malloc(sizeof(char) * strlen(is->fields[1]) + 1); //Tach ten tuyen 
            strcpy(busLine, is->fields[1]);//field 0 la [TUYEN], fields la ex: 03,04.
            continue;
        }

        int busStopNum = 0;              //So luong diem bus trong line
        for (int i = 0; i < is->NF; i++) //NF: so luong field trong line: so tu trong line
        {
            if (strcmp(is->fields[i], "-") == 0)
                busStopNum++; 
        }

        char *busStopName[++busStopNum]; //mang luu ten cac diem bus
        int j = 0; //bien chay de luu cac field vao busStop

        busStopName[j] = (char *)malloc(sizeof(char) * 100);
        for (int i = 0; i < is->NF; i++)
        {
            if (strcmp(is->fields[i], "-") == 0)
            {
                j++; //thu tu diem bus trong line
                busStopName[j] = (char *)malloc(sizeof(char) * 100);
                continue;
            }
            else
            {
                strcat(busStopName[j], is->fields[i]);
                if (i < is->NF - 1)
                    if (strcmp(is->fields[i + 1], "-") != 0)
                        strcat(busStopName[j], " ");
            }
        }

        for (int i = 0; i < busStopNum; i++)
            addVertex(g, busStopName[i], busLine);
        for (int i = 0; i < busStopNum - 1; i++)
            addEdge(g, busStopName[i], busStopName[i + 1]);
    }
}

Graph createGraph()
{
    Graph graph;

    graph.edges = make_jrb();
    graph.vertices = make_jrb();

    return graph;
}

void dropGraph(Graph graph)
{
    JRB node, tree;

    jrb_traverse(node, graph.edges)
    {
        tree = (JRB)jval_v(node->val);
        jrb_free_tree(tree);
    }
    jrb_free_tree(graph.edges);
    jrb_free_tree(graph.vertices);
}

void addVertex(Graph graph, char *name, char *busLine)
{
    JRB node = jrb_find_str(graph.vertices, name);
    JRB tree;
    if (node == NULL)
    {
        tree = make_jrb();
        jrb_insert_str(graph.vertices, strdup(name), new_jval_v((void *)tree));
    }
    else
    {
        tree = (JRB)jval_v(node->val);
    }

    JRB node1 = jrb_find_str(tree, busLine);
    if(node1 == NULL)
        jrb_insert_str(tree, strdup(busLine), new_jval_i(1));
}


void display(Graph graph)
{
    JRB node;

        jrb_traverse(node, graph.vertices)
        {
            printf("%s\n", jval_s(node->key));
        }
}

char *getVertex(Graph graph, int id)
{
    JRB node;
    int i = 1;

    jrb_traverse(node, graph.vertices)
    {
        if(i == id)
           return jval_s(node->key);
        else
            i++;
    }

    return NULL;
}

int hasEdge(Graph graph, char *v1, char *v2)
{
    JRB node, tree;

    node = jrb_find_str(graph.edges, v1);
    if(node == NULL)
        return 0;
    tree = (JRB)jval_v(node->val);
    if(jrb_find_str(tree, v2) == NULL)
        return 0;
    else
        return 1;
}

void addEdge(Graph graph, char *v1, char *v2)
{
    JRB node, tree;
    if(!hasEdge(graph, v1, v2))
    {
        node = jrb_find_str(graph.edges, v1);
        if(node == NULL)
        {
            tree = make_jrb();
            jrb_insert_str(graph.edges, strdup(v1), new_jval_v(tree));
        }
        else
        {
            tree = (JRB)jval_v(node->val);
        }
        jrb_insert_str(tree, strdup(v2), new_jval_i(1));
    }
}

int getNumberOfVertex(Graph graph)
{
    JRB node;
    int count = 0;

    jrb_traverse(node, graph.vertices)
    {
        count++;
    }
    return count;
}

int getNumberOfEdge(Graph graph)
{
    JRB node;
    int count = 0;

    jrb_traverse(node, graph.edges)
    {
        count++;
    }
    return count;
}

int shortest_Path(Graph graph, char *start, char* stop, char *path[])
{
    JRB distance; 
    distance = make_jrb();
    char *u; 
    Dllist queue, node; 

    queue = new_dllist(); 
    dll_append(queue, new_jval_s(start));

    Dllist trace = new_dllist(); //trace : queue luu lai duong di phia truoc = previous
    dll_append(trace, new_jval_s(start));
    jrb_insert_str(distance, start, new_jval_v(trace)); //trace la duong di den tung dinh s den dinh dang xet = previous(s)

    while (!dll_empty(queue))
    {
        node = dll_first(queue); 

        u = (char *)malloc(sizeof(char) * strlen(jval_s(node->val)) + 1);
        strcpy(u, jval_s(node->val));
        dll_delete_node(node);

        JRB temp = jrb_find_str(distance, u);
        Dllist u_find_path = jval_v(temp->val); 

        if (strcmp(u, stop) == 0)
        {
            int count = 0; 
            Dllist tmp;

            dll_traverse(tmp, u_find_path)
            {
                count++;
                if(path != NULL)
                {
                    path[count -1] = (char *)malloc(sizeof(char) * strlen(jval_s(tmp->val)) + 1);
                    strcpy(path[count - 1], jval_s(tmp->val));
                }
            }
            return count;
        }

        char *output[1000];

        int n = outdegree(graph, u, output); 

        for (int i = 0; i < n; i++)
        {
            JRB vertex_find = jrb_find_str(distance, output[i]);

            if (vertex_find == NULL) 
            {
                Dllist cur_path = new_dllist(); 
                Dllist node1;
                dll_traverse(node1, u_find_path) 
                    dll_append(cur_path, node1->val);

                dll_append(cur_path, new_jval_s(output[i]));
                jrb_insert_str(distance, output[i], new_jval_v(cur_path));
                dll_append(queue, new_jval_s(output[i]));
            }
        }
    }
    return INFINITIVE_VALUE;
}

int indegree(Graph graph, char *v, char *output[])
{
    JRB tree, node;
    int total = 0;

    jrb_traverse(node, graph.edges)
    {
        tree = (JRB)jval_v(node->val);
        if(jrb_find_str(tree, v))
        {
            output[total] = (char *)malloc(sizeof(char) * strlen(jval_s(node->key)) + 1);
            output[total] = jval_s(node->key);
            total++;
        }
    }
    return total;
}

int outdegree(Graph graph, char *v, char *output[])
{
    JRB tree, node;
    int total = 0;

    node = jrb_find_str(graph.edges, v);

    if(node == NULL)
        return 0;

    tree = (JRB)jval_v(node->val);

    jrb_traverse(node, tree)
    {
        output[total] = (char *)malloc(sizeof(char) * strlen(jval_s(node->key)) + 1);
        strcpy(output[total], jval_s(node->key));
        total++;
    }
    return total;
}

int getAdjacents(Graph graph, char *v)
{
    JRB node;
    int count = 0;

    jrb_traverse(node, graph.vertices)
    {
        char *u = jval_s(node->key);

        if(hasEdge(graph, v, u) == 1 || hasEdge(graph, u, v) == 1)
        {
            printf("\t%2d. [%s]\n",count + 1, u);
            count++;
        }
    }
    return count;
}