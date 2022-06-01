#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>

#define mapheight 26
#define mapwidth 120

int i, j;
int brick_lenght;
char map[mapheight][mapwidth+1];

typedef struct Sobject//��������� ��������� ���������� ��������� �� �����
{
    float x, y;//����������
    float width, height;//������ �� ������
    float speed_vert;//���
    BOOL space_controle;//�������� �������
    char symbole;//��� ���� ������������ �������� ��� ����
} Tobject;
Tobject rabbit;//��� ��������
Tobject *brick=NULL;//�����

BOOL check_map(int x, int y)//�������� ��������� � ������ ����
{
    return ((x>=0)&&(x<mapwidth)&&(y>=0)&&(y<mapheight));
}

BOOL check_collaps(Tobject o1, Tobject o2)//�������� �� �������� � ������
{
    return ((o1.x + o1.width)>o2.x)&&(o1.x < (o2.x + o2.width))&&
           ((o1.y + o1.height)>o2.y)&&(o1.y < (o2.y + o2.height));
}

void setcur(int x, int y)//������������ ������� (��������� ��� ���������� ������ �� ����)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void map_clear()//��������� ����
{
    for (i = 0; i<mapwidth; i++)
        map[0][i]=' ';
    map[0][mapwidth]= '\0';
    for (j = 1; j<mapheight; j++)
        sprintf(map[j],map[0]);
}

void map_out()//�������� ����
{
    map[mapheight-1][mapwidth-1]='\0';
    for (j = 0; j<mapheight; j++)
        printf("%s",map[j]);
}

void position(Tobject *obj, float xpos, float ypos)//������ �������(����������)
{
    (*obj).x = xpos;
    (*obj).y = ypos;
}
void init(Tobject *obj, float xpos, float ypos, float rwidth, float rheight, char rsymbole)//�����������
{
    position(obj, xpos, ypos);
    (*obj).width = rwidth;
    (*obj).height = rheight;
    (*obj).speed_vert = 0;
    (*obj).symbole = rsymbole;//������, ���� �� ������ "��������" ��'���
}

void out(Tobject obj)//����
{
    //��������� ���� �� ������������� ������� ����������, ������� ���������� ����� ����� ���
    int ix = (int)round(obj.x);
    int iy = (int)round(obj.y);
    int iwidth = (int)round(obj.width);
    int iheight = (int)round(obj.height);

    for (i = ix; i<(ix + iwidth); i++)
        for (j = iy; j < (iy + iheight); j++)
            if(check_map(i, j))
                map[j][i]= obj.symbole;//���� ������� �� ����
}
void level();
void speed_vert(Tobject *obj)//����������� ��������
{
    (*obj).space_controle = TRUE;
    (*obj).speed_vert += 0.05;//������ ��������(�� ��������, ��� �������)
    position(obj, (*obj).x, (*obj).y+(*obj).speed_vert);//������ �� �������� � �����������
    for (i=0; i<brick_lenght; i++)
        if (check_collaps(*obj, brick[i]))//�������� �� �������� � ������
        {
            (*obj).y -=(*obj).speed_vert;//������ ��'���� ��������, ����� �������� ����� (�������)
            (*obj).speed_vert = 0;//��������� ��������� �� ���� (��� ���������: ��������� ���� ������)
            (*obj).space_controle = FALSE;//�������� ����� �� �����
            if (brick[i].symbole=='|')//���� �������� ���������� �� ����� � ��������� �������,
                                      //��� ���������� � �������
            {
                Sleep(1000);
                level();
            }
            break;
        }
}

void map_move_hor(float dx)//������ ���� ������������� (����� � �� �������� �� ��������, � ����)
{
    rabbit.x -= dx;//�������� ����
    for (i=0; i<brick_lenght; i++)
        if (check_collaps(rabbit, brick[i]))
        {
            rabbit.x+=dx;//��������� ���� �� ��������
            return;//��� ���������� � �������
        }
    rabbit.x += dx;
    for (i=0; i<brick_lenght; i++)
        brick[i].x += dx;//���� ��� ����
}

void level()//����� ���, ��� ����� �������� ���������, ������ ������ ����� �� ������� �� ����
{
    init(&rabbit,39, 10, 3, 3,'/');

    brick_lenght = 8;
    brick = realloc( brick, sizeof(*brick) * brick_lenght );//�������� ������ ��� �����
    init(brick+0, 20, 20, 40, 5, '_');
    init(brick+1, 80, 20, 15, 10,'_');
    init(brick+2, 110, 20, 20, 5,'_');
    init(brick+3, 140, 15, 10, 10,'_');
    init(brick+4, 170, 20, 20, 5,'_');
    init(brick+5, 200, 20, 15, 15,'_');
    init(brick+6, 230, 20, 20, 5,'_');
    init(brick+7, 270, 20, 20, 5,'|');//������� ���� ���� ���� ��� ���������� � �������
}

void game()//��'������ �������, �������� "�����" ��� ��� ����������
{
    system("color 6F");//������ ���� �� �������� ����
    level();
    do
    {
        map_clear();
        //�������� �� ��������� ������ "�����"
        if ((rabbit.space_controle == FALSE)&&(GetKeyState(VK_SPACE)<0)) rabbit.speed_vert=-1;
        if (GetKeyState('A')<0)map_move_hor(1);//�������� ���� �� ��������� ������ �
        if (GetKeyState('D')<0)map_move_hor(-1);//�������� ���� �� ��������� ������ D
        if (rabbit.y > mapheight)//����������� � �������, ���� �������� ����
        {
            Sleep(1000);
            level();
        }
        speed_vert(&rabbit);//��� �������� ����� �� ����� (�������� ��������)
        //���� �����
        out(brick[0]);
        out(brick[1]);
        out(brick[2]);
        out(brick[3]);
        out(brick[4]);
        out(brick[5]);
        out(brick[6]);
        out(brick[7]);
        out(brick[8]);

        out(rabbit);//���� ���������
        setcur(0,0);//������������ ������
        map_out();//�������� ����
    }
    while (GetKeyState(VK_ESCAPE)>=0);//����� � ��� �� ��������� ������ esc
}

void menu()//���������
{
    system("color 6");
    char str [] = "\nHello, my friend\nSeems like you wanna play a little game\nWould you like to read the rules first?";
    for (int i = 0; i < strlen (str); i++)//����������� ����
    {
        Sleep(30);
        printf("%c",str[i]);
    }
    int a, b;
    printf("\n1 - yes\n2 - no\n");
    scanf("%d", &a);
    switch (a)
    {
    case 1://�������
        system("color 8");
        printf("\n\nSo the rules are really easy:\n\nJump between the blocks and try not to fall in the deep,\n\n");
        printf("But if you do so, you will start the level from the very beginning.\n");
        printf("\nYou can control your character with A, D and space.\n");
        printf("\nGo to the last block and receive a victory.\n");
        printf("\nI believe in you, my friend!\n\n");
        printf("\n\npress 1 - if you are ready to go...\n");
        scanf("%d", &b);
        switch(b)
        {
        case 1:
            game();
            break;
        default :
            game();
        }
        break;

    case 2://���
        game();
        break;
    }
}

int main()
{
    menu();
    return 0;
}
