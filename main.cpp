#include <iostream>
#include <stack>
#include <queue>
#include <string.h>
using namespace std;
stack<char>s;
stack<char>stemp;
stack<char>stempd;
stack<char>temp;
queue<char>q;
queue<char>qtemp;
queue<char>firsttemp;
queue<char>followtemp;
#define MAX 10
#define TRUE 1;
#define FLASE 0;
int firstNum=0;
const int gcount=5;
char *tab[MAX][MAX];
char *first[MAX];
char tempfirst[MAX][MAX]={'\0'};
char *follow[MAX];
char tempfollow[MAX][MAX]={'\0'};
char *gram[MAX][MAX];
char *regram[MAX];
char tkey[MAX];
char nkey[MAX];
int line=1;
int getTnum(char Word);
void printgram();
int IsEKey(char *Word);
int IsTKey(char Word);
int Isybxl(char *Word);
int getNnum(char Word);
void printfirst();
void initregram(){
    char *temp;
    cout<<"请输入文法(end结束):"<<endl;
    for(int i=0;i<MAX;i++){
        temp = new char;
        cout<<"第"<<i+1<<"条文法:";
        cin>>temp;
        if(strcmp(temp,"end")==0){
            break;
        }
        regram[i]=temp;
    }
}

void initgram(){
    for(int i=0;i<MAX;i++){
        for(int j=0;j<MAX;j++){
            gram[i][j]="err";
        }
    }
    int j,a,b,i;
    char *temp;

    for(i=0;i<5;i++){
        j=3;
        a=0;
        b=0;
        temp = new char;
        for(int c=0;c<10;c++){
            temp[c]='\0';
        }
        while(regram[i][j]!='\u0000'){
            if(regram[i][j]=='|'){
                gram[i][a]=temp;
                temp = new char;
                for(int c=0;c<10;c++){
                    temp[c]='\0';
                }
                a++;
                b=0;
                j++;
                continue;
            }
            temp[b]=regram[i][j];
            b++;
            j++;
        }
        gram[i][a]=temp;
    }
}

void inittab(){
    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            tab[i][j]="err";
        }
    }
    for(int i=0;i<gcount;i++){
        int j=0;
        int ybxl=FLASE;
        while(first[i][j]!='\u0000'){
            if(first[i][j]=='$'){   //如果是ybxl，不入队列
                ybxl=TRUE;
                j++;
                continue;
            }
            firsttemp.push(first[i][j]);
            j++;
        }
        while(!firsttemp.empty()){
            tab[i][getTnum(firsttemp.front())]=gram[i][0]; //填入第0项
            int a=0;
            while(strcmp(gram[i][a],"err")!=0){
                if(gram[i][a][0]==firsttemp.front()){ //语法的第一个字符，相等
                    tab[i][getTnum(firsttemp.front())]=gram[i][a];
                    break;
                }
                a++;
            }
            firsttemp.pop();
        }
        if(ybxl){
            j=0;
            while(follow[i][j]!='\u0000'){
                followtemp.push(follow[i][j]);
                j++;
            }
            while(!followtemp.empty()){
                tab[i][getTnum(followtemp.front())]="ε";
                followtemp.pop();
            }
        }
    }
}

void getNkey(){
    for(int i=0;i<MAX;i++){
        nkey[i]='\0';
    }
    for(int i=0;i<5;i++){
        nkey[i]=regram[i][0];
    }
}

void getTkey(){
    for(int i=0;i<MAX;i++){
        tkey[i]='\0';
    }
    int b=0;
    for(int i=0;i<MAX;i++){
        if(!IsEKey(gram[i][0])){
            tkey[b]='#';
            break;
        }
        for(int j=0;j<MAX;j++){
            if(!IsEKey(gram[i][j])){
                break;
            }
            int a=0;
            while(gram[i][j][a]!='\u0000'){
                if(!(gram[i][j][a]>=65&&gram[i][j][a]<=90)){
                    if(Isybxl(&gram[i][j][a])){
                        break;
                    }
                    if(IsTKey(gram[i][j][a])){
                        tkey[b]=gram[i][j][a];
                        b++;
                    }
                }
                a++;
            }
        }
    }
}

char* getFirst(int i){
    char first_t[MAX]={'\0'},t;
    firstNum=0;
    int j=0;
    while(IsEKey(gram[i][j])){
        if(gram[i][j][0]>='A'&&gram[i][j][0]<='Z'){ //非终结符
            memcpy(first_t,getFirst(getNnum(gram[i][j][0])), sizeof(getFirst(getNnum(gram[i][j][0]))));
        }
        else{
            if(Isybxl(gram[i][j]))
                t='$';
            else{
                t=gram[i][j][0];
            }
        }
        first_t[firstNum]=t;
        firstNum++;
        j++;
    }
    return first_t;
}

void writeFirst(){
    int i=0;
    char *t;
    while(nkey[i]!='\0'){
        t = new char;
        t=getFirst(i);
        int j=0;
        while(*t!='\0'){
            tempfirst[i][j]=*t;
            t++;
            j++;
        }
        first[i]=tempfirst[i];
        i++;
    }
}

char* getFollow(int a){
    char c,follow_t[MAX]={'\0'};
    char *temp;
    int exist;
    int f=0;
    c = nkey[a];
    if(a==0){
        follow_t[f]='#';
        f++;
    }
    for(int i=0;i<MAX;i++){
        if(strcmp(gram[i][0],"err")==0)
            break;
        for(int j=0;j<MAX;j++){
            if(strcmp(gram[i][j],"err")==0)
                break;
            if(strcmp(gram[i][j],"$")==0)   //ybxl跳过
                break;
            for(int k=1;k<MAX-1;k++){   //第一个和最后一个不判断 因为A->aB A->aBb
                if(gram[i][j][k]=='\0')
                    break;
                if(gram[i][j][k]==c){
                    exist=FLASE;
                    if(gram[i][j][k+1]=='\0'){  //最后一个是非终结符 A->aB
                        if(nkey[i]!=c){
                            temp=follow[i];
                            while(*temp!='\0'){
                                for(int b=0;b<f;b++){  //终结符是否已经存在follow集中
                                    if(follow_t[b]==*temp){
                                        exist=TRUE;
                                        break;
                                    }
                                }
                                if(!exist){
                                    follow_t[f]=*temp;
                                    f++;
                                }
                                temp++;
                                exist=FLASE;
                            }
                        }
                    }
                    else if(gram[i][j][k+1]>='A'&&gram[i][j][k+1]<='Z'){ //非终结符
                        temp=first[getNnum(gram[i][j][k+1])];
                        while(*temp!='\0'){
                            if(*temp=='$'){
                                temp++;
                                continue;
                            }
                            for(int b=0;b<f;b++){  //终结符是否已经存在follow集中
                                if(follow_t[b]==*temp){
                                    exist=TRUE;
                                    break;
                                }
                            }
                            if(!exist){
                                follow_t[f]=*temp;
                                f++;
                            }
                            temp++;
                            exist=FLASE;
                        }
                        int d=0;
                        while(strcmp(gram[getNnum(gram[i][j][k+1])][d],"err")!=0) {   //跟在后面的非终结符能否推出ybxl
                            if(Isybxl(gram[getNnum(gram[i][j][k+1])][d])){
                                temp=follow[i];
                                while(*temp!='\0'){
                                    for(int b=0;b<f;b++){  //终结符是否已经存在follow集中
                                        if(follow_t[b]==*temp){
                                            exist=TRUE;
                                            break;
                                        }
                                    }
                                    if(!exist){
                                        follow_t[f]=*temp;
                                        f++;
                                    }
                                    temp++;
                                    exist=FLASE;
                                }
                            }
                            d++;
                        }
                    }
                    else{   //终结符
                        for(int b=0;b<f;b++){  //终结符是否已经存在follow集中
                            if(follow_t[b]==gram[i][j][k+1]){
                                exist=TRUE;
                                break;
                            }
                        }
                        if(!exist){
                            follow_t[f]=gram[i][j][k+1];
                            f++;
                        }
                        exist=FLASE;
                    }
                }
            }
        }
    }
    return follow_t;
}

void writeFollow(){
    int i=0;
    char *t;
    while(nkey[i]!='\0'){
        t = new char;
        t=getFollow(i);
        int j=0;
        while(*t!='\0'){
            tempfollow[i][j]=*t;
            t++;
            j++;
        }
        follow[i]=tempfollow[i];
        i++;
    }
}

int IsTkey(char Word){      //判断关键字
    for(int i=0;i<6;i++){
        if(Word==tkey[i]){ //比较字符串
            return TRUE;
        }
    }
    return FLASE;
}

int getTnum(char Word){
    for(int i=0;i<6;i++){
        if(Word==tkey[i]){ //比较字符串
            return i;
        }
    }
}

int getNnum(char Word){
    for(int i=0;i<gcount;i++){
        if(Word==nkey[i]){ //比较字符串
            return i;
        }
    }
}

int IsTKey(char Word){      //判断关键字
    int i;
    for(i=0;i<MAX;i++){
        if(tkey[i]==Word){ //比较字符串
            return FLASE;
        }
    }
    return TRUE;
}

int IsEKey(char *Word){      //判断关键字
    char *e ="err";
    if(strcmp(Word,e)==0){ //比较字符串
        return FLASE;
    }
    return TRUE;
}

int Isybxl(char *Word){
    char *e = "ε";
    if(strcmp(Word,e)==0){
        return TRUE;
    }
    return FLASE;
}

void printfirst(){
    cout<<"FIRST集如下:"<<endl;
    for(int i=0;i<gcount;i++){
        int j=0;
        cout<<"FIRST("<<nkey[i]<<"):{";
        while(first[i][j]!='\u0000'){
            cout<<first[i][j];
            if(first[i][j+1]!='\u0000')
                cout<<',';
            j++;
        }
        cout<<'}'<<endl;
    }
}

void printfollow(){
    cout<<"FOLLOW集如下:"<<endl;
    for(int i=0;i<gcount;i++){
        int j=0;
        cout<<"FOLLOW("<<nkey[i]<<"):{";
        while(follow[i][j]!='\u0000'){
            cout<<follow[i][j];
            if(follow[i][j+1]!='\u0000')
                cout<<',';
            j++;
        }
        cout<<'}'<<endl;
    }
}

void printtab(){
    cout<<"分析表如下:"<<endl;
    cout<<"\t\t\t";
    for(int i=0;i<6;i++){
        cout<<tkey[i]<<"\t\t\t";
    }
    cout<<endl;
    for(int i=0;i<gcount;i++){
        cout<<nkey[i]<<"\t\t\t";
        cout<<tab[i][0]<<"\t\t\t"<<tab[i][1]<<"\t\t\t"<<tab[i][2]<<"\t\t\t"<<tab[i][3]<<"\t\t\t"<<tab[i][4]<<"\t\t\t"<<tab[i][5]<<endl;
    }
}

void printstack(){
    while(!stemp.empty()){
        stempd.push(stemp.top());
        stemp.pop();
    }
    cout<<line<<'\t'<<'\t';
    line++;
    while(!stempd.empty()){
        cout<<stempd.top();
        stempd.pop();
    }
}

void printqueue(){
    cout<<'\t'<<'\t';
    while(!qtemp.empty()){
        cout<<qtemp.front();
        qtemp.pop();
    }
    cout<<endl;
}

void printgram(){
    cout<<"转换后的文法如下:"<<endl;
    for(int i=0;i<MAX;i++){
        for(int j=0;j<MAX;j++){
            if(strcmp(gram[i][j],"err")==0)
                break;
            cout<<"gram["<<i<<"]["<<j<<"]:"<<gram[i][j]<<endl;
        }
        if(strcmp(gram[i][0],"err")==0)
            break;
    }
}

void exe(){
    int flag = TRUE;
    int tnum,nnum;
    char stack;
    cout<<"分析过程如下:"<<endl;
    s.push('#');
    s.push('E');
    stemp=s;
    printstack();
    qtemp=q;
    printqueue();
    while(flag){
        stack = s.top();
        s.pop();
        if(IsTkey(stack)&&stack!='#'){
            if(stack==q.front())
                q.pop();
            else
                cout<<"error"<<endl;
        }
        else if(stack=='#'){
            if(stack==q.front()){
                flag = FLASE;
                continue;
            }
            else
                cout<<"error"<<endl;
        }
        else{
            tnum = getTnum(q.front());
            nnum = getNnum(stack);
            if(IsEKey(tab[nnum][tnum])){
                int i=0;
                if(Isybxl(tab[nnum][tnum])){
                }
                else{
                    while(tab[nnum][tnum][i]!='\0'){
                        temp.push(tab[nnum][tnum][i]);
                        i++;
                    }
                    while(!temp.empty()){
                        s.push(temp.top());
                        temp.pop();
                    }
                }
            }
            else
                cout<<"error"<<endl;
        }
        stemp=s;
        printstack();
        qtemp=q;
        printqueue();
    }
}

void initchar(){
    char a[MAX];
    int i=0;
    cout<<"请输入要分析的句子:";
    cin>>a;
    while(a[i]!='\0'){
        q.push(a[i]);
        i++;
    }
}

int main() {
    /*
     * E->TR
     * R->+TR|ε
     * T->FY
     * Y->*FY|ε
     * F->(E)|i
     * (i+i)*i#
     */
    initregram();
    initchar();
    initgram();
    printgram();
    getNkey();
    getTkey();
    writeFirst();
    writeFollow();
    inittab();
    printfirst();
    printfollow();
    printtab();
    exe();
    return 0;
}