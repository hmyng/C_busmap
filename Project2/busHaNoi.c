#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jrb.h"
#include "dllist.h"
#include "fields.h"

#define INFINITIVE_VALUE 10000000

#define max 1000
#define sizemax 1000

typedef struct{
    JRB edges;
    JRB vertices;
} Graph;

Graph createGraph();
void dropGraph(Graph graph);
void printBus(Graph graph);
void addVertex(Graph graph, char *name, char *busLine);
char *getVertex(Graph graph, int id);
int hasEdge(Graph graph, char *v1, char *v2);
void addEdge(Graph graph, char *v1, char *v2);
int getNumOfVertex(Graph graph);
int getNumOfEdge(Graph graph);
int indegree(Graph graph, char *v, char *output[]);
int outdegree(Graph graph, char *v, char *output[]);
int getAdjacents(Graph graph, char *v);
int shortedPath(Graph graph, int s, int t, int *path, int *length);


void readData(Graph g, char *filename);
void instruction();


int main()
{
    int key = 1, size, total, id, i;
    char name[max], s1[max], s2[max], busLine[6], bus[4];
    //char *output[max];

    char *filename = "bus.txt";
    Graph graph;

    graph = createGraph();

    readData(graph, filename);

    instruction();
    printf("Nhap lua chon cua ban: ");
    scanf("%d%*c", &key);

    while (key != 0)
    {
        switch (key)
        {
        case 1:
            printf("\n\t____Ten mot dia diem trong Ha Noi___\n\n");
            printf("Nhap so hieu diem diem muon tim ten: ");
            scanf("%d%*c", &id);
            size = getNumOfVertex(graph);
            if(id > size || id <= 0)
                printf("Khong ton tai so hieu diem %d\nSo hieu diem nam trong doan tu 1 den %d\n", id, size);
            else
                printf("Ten cua dia diem %d la: \n\t[%s]\n", id, getVertex(graph, id));
            printf("\n=============================================================\n");
            break;

        case 2:
            printf("\n\t___Kiem tra hai diem co noi truc tiep voi nhau___\n\n");
            puts("Nhap ten 2 diem ban muon kiem tra: ");
            printf("\tDiem 1: ");
            gets(s1);
            printf("\tDiem 2: ");
            gets(s2);
            JRB node1, node2, tree1, tree2;
            //printf("\n");
            node1 = jrb_find_str(graph.vertices, s1);
            node2 = jrb_find_str(graph.vertices, s2);
            if (node1 == NULL)
                printf("Khong ton tai diem  [%s]\n", s1);
            if (node2 == NULL)
                printf("Khong ton tai diem  [%s]\n", s2);

            if(hasEdge(graph, s1, s2) == 1 || hasEdge(graph, s2, s1) == 1)
            {
                puts("---> Hai diem tren noi truc tiep voi nhau");
                tree1 = (JRB)jval_v(node1->val);
                tree2 = (JRB)jval_v(node2->val);

                printf("---> Tuyen noi truc tiep 2 diem [%s] va [%s] la: \n", s1, s2);
                JRB node;

                jrb_traverse(node, tree1)
                {
                    if (jrb_find_str(tree2, jval_s(node->key)) != NULL)
                        printf("\t%s\n", jval_s(node->key));
                }
            }
            else
                puts("---> Hai diem trem khong noi truc tiep voi nhau");


            printf("\n=============================================================\n");
            break;

        case 3:
            puts("\n\t___Cac dia diem noi truc tiep voi mot dia diem___\n");

            printf("Nhap ten diem: ");
            rewind(stdin);
            gets(name);
            JRB node = jrb_find_str(graph.vertices, name);

            if (node == NULL)
                printf("Khong ton tai ten dia diem [%s]\n", name);
            else
            {
                printf("---> Cac dia diem noi truc tiep voi [%s] la: \n", name);

                total = getAdjacents(graph, name);
                printf("\n---> Co %d diem noi truc tiep\n", total);
            }

            printf("\n=============================================================\n");
            break;

        case 4:
            puts("\n\t___Tim duong di ngan nhat giua hai dia diem___\n");
            puts("Nhap ten hai dia diem: ");
            printf("\tDiem 1: ");
            rewind(stdin);
            gets(s1);
            printf("\tDiem 2: ");
            rewind(stdin);
            gets(s2);

            if(jrb_find_str(graph.vertices, s1) == NULL)
            {   printf("Khong ton tai diem [%s]\n", s1);
                break;
            }
            if(jrb_find_str(graph.vertices, s2) == NULL)
            {
                printf("Khong ton tai diem [%s]\n", s2);
                break;
            }

            char *path[2000];
            int num_path = shortest_Path(graph, s1, s2, path);
            if(num_path != INFINITIVE_VALUE)
            {
                printf("\n*** Duong di ngan nhat giua [%s] va [%s] la:  \n", s1, s2);

                for (i = 0; i < num_path; i++)
                {
                    printf("[%s] ", path[i]);
                    if(i < num_path - 1)
                        printf("--> ");
                }
                printf("\n");
                puts("\n*** Lo trinh tuyen can di \n");

                for (i = 0; i < num_path-1; i++)
                {
                    printf("*[%s] --> [%s]: \n", path[i], path[i+1]);

                    JRB node1 = jrb_find_str(graph.vertices, path[i]);
                    JRB node2 = jrb_find_str(graph.vertices, path[i + 1]);

                    JRB tree1 = (JRB)jval_v(node1->val);
                    JRB tree2 = (JRB)jval_v(node2->val);

                    printf("Tuyen di qua \n");
                    JRB node;

                    jrb_traverse(node, tree1)
                    {
                        if(jrb_find_str(tree2, jval_s(node->key)) != NULL)
                            printf("\t%s\n", jval_s(node->key));
                    }
                }
            }
            else
                printf("Khong tim duoc duong di tu [%s] den [%s]\n", s1, s2);

            printf("\n=============================================================\n");
            break;

        case 5:
            printf("\n\t___Liet ke cac tuyen bus di qua mot diem___\n\n");
            printf("Nhap ten diem: ");
            rewind(stdin);
            gets(name);

            JRB node_Find = jrb_find_str(graph.vertices, name);
            if(node_Find == NULL)
                printf("Khong ton tai ten diem: [%s]\n", name);
            else
            {
                printf("Cac tuyen bus di qua diem [%s] la: \n", name);
                JRB tree = (JRB)jval_v(node_Find->val);
                JRB node;

                jrb_traverse(node, tree)
                    printf("\t%s\n", jval_s(node->key));
            }


            printf("\n=============================================================\n");
            break;

        case 6:
            printf("\n\t___Liet ke cac diem bus mot tuyen bus di qua___\n\n");
            printf("Nhap ten tuyen bus: ");
            rewind(stdin);
            gets(bus);
            strcpy(busLine, "[");
            strcat(busLine, bus);
            strcat(busLine, "]");

            JRB node_v, tree;
            jrb_traverse(node_v, graph.vertices)
            {
                tree = (JRB)jval_v(node_v->val);
                if(jrb_find_str(tree, busLine) != NULL)
                    printf("\t[%s]\n", jval_s(node_v->key));
            }

            printf("\n=============================================================\n");
            break;

        default:
            puts("Khong ton tai lua chon cua ban !\nVui long nhap lai !\n");
            break;
        }
        instruction();
        printf("Nhap lua chon cua ban: ");
        scanf("%d%*c", &key);
    }
    printf("Ket thuc chuong trinh !\n");
    printf("\n=============================================================\n");

    return 0;
}


void readData(Graph g, char *filename)
{
    IS is = new_inputstruct(filename);

    if (is == NULL)
    {
        printf("File fail!\n");
        exit(1);
    }

    char *busLine;//ten tuyen

    while (get_line(is) > 0) //Doc tung dong trong file
    {
        if (strcmp(is->fields[0], "TUYEN") == 0) //kiem tra dong chua ten tuyen
        {
            busLine = (char *)malloc(sizeof(char) * strlen(is->fields[1]) + 1); //Tach ten tuyen ra
            strcpy(busLine, is->fields[1]);
            continue;
        }

        int busStopNum = 0;              //So luong diem bus trong line
        for (int i = 0; i < is->NF; i++) //NF: so luong field trong line: so tu trong line
        {
            if (strcmp(is->fields[i], "-") == 0)
                busStopNum++; //Dem xem co bao nhieu ki tu - trong line
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

void instruction()
{
    printf("=============================================================\n");
    printf("\n  ********CHUONG TRINH TIM DUONG DI XE BUS HA NOI********  \n\n");
    printf("=============================================================\n\n");
    puts("  [ 1 ] Ten cua mot dia diem trong Ha Noi                    \n");
    puts("  [ 2 ] Kiem tra hai dia diem co noi truc tiep voi nhau khong\n");
    puts("  [ 3 ] Cac dia diem noi truc tiep voi mot dia diem          \n");
    puts("  [ 4 ] Tim duong di ngan nhat giua hai dia diem             \n");
    puts("  [ 5 ] Liet ke tat ca cac tuyen bus di qua mot diem         \n");
    puts("  [ 6 ] Liet ke tat ca cac diem bus mot tuyen bus di qua     \n");
    puts("  [ 0 ] Thoat khoi chuong trinh                              \n");
    printf("=============================================================\n");
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


void printBus(Graph graph)
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

int getNumOfVertex(Graph graph)
{
    JRB node;
    int count = 0;

    jrb_traverse(node, graph.vertices)
    {
        count++;
    }
    return count;
}

int getNumOfEdge(Graph graph)
{
    JRB node;
    int count = 0;

    jrb_traverse(node, graph.edges)
    {
        count++;
    }
    return count;
}

int shortest_Path(Graph graph, char *s, char* t, char *path[])//BFS
{
    // Khoi tao cac distance = 0
    JRB distance; //Cay luu path, luu ten dinh duyet cung queue phia truoc
    distance = make_jrb();
    char *u; //Bien trung gian de lm cau
    Dllist queue, node; //queue de luu

    queue = new_dllist(); //
    dll_append(queue, new_jval_s(s));

    Dllist trace = new_dllist(); //truy vet: trace la mot queue luu lai duong di phia truoc = previous
    dll_append(trace, new_jval_s(s));
    jrb_insert_str(distance, s, new_jval_v(trace)); //trace la duong di den tung dinh s den dinh dang xet = previous(s)

    // Duyet Queue
    while (!dll_empty(queue))
    {
        node = dll_first(queue); //dequeue

        u = (char *)malloc(sizeof(char) * strlen(jval_s(node->val)) + 1);
        strcpy(u, jval_s(node->val));
        dll_delete_node(node);

        JRB temp = jrb_find_str(distance, u);
        Dllist u_find_path = jval_v(temp->val); //tim kiem trong dis gia tri dinh u, val la trace : queue luu duong

        if (strcmp(u, t) == 0)
        {
            //Truy vet lai path
            int count = 0; //bien dem so diem di qua
            Dllist tmp;//List luu path

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

        char *output[2000];

        int n = outdegree(graph, u, output); //Cac dinh ma u huong vao

        // Cap nhap distance cua tat ca cac dinh ma lien ke voi dinh min
        for (int i = 0; i < n; i++)
        {
            JRB vertex_find = jrb_find_str(distance, output[i]);

            if (vertex_find == NULL) //Chua tham vertex
            {
                Dllist cur_path = new_dllist(); //Luu duong di cho vertex_find tu s
                Dllist node1;
                dll_traverse(node1, u_find_path) //u_find_path la dinh phia truoc vua duyet o tren
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