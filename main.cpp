/*#include <iostream>
#include<set>
#include<unordered_map>
using namespace std;
bool ifstart = false,frozen=false;
struct team{
    string name;
    int AC_time[27]={0};
    int all_AC_time[27]={0};
    int rank;
    int problem_solve=0;
    int all_solved;
    int penalty=0;
    int all_penalty;
    char latest_submission;
    set<int>MAXPASSTIME;
    set<int>AC_TIME;
    set<int>ALL_AC_TIME;
    unordered_map<string,char>status_to_problm;
    unordered_map<char,bool>freeze;
    unordered_map<char,int>submit_times;
    unordered_map<char,int>all_submit;
    unordered_map<char,string>state;
    unordered_map<char,string>frez_state;
    unordered_map<string,char>problem;
    unordered_map<char,unordered_map<string,int>>specific_state;
    //int MAXPASSTIME=0;
} ;
unordered_map<string , team> T;
struct compare{
    string set_name;
    bool operator<(const compare &b) const {
        if(T[set_name].problem_solve>T[b.set_name].problem_solve)return true;
        if(T[set_name].problem_solve<T[b.set_name].problem_solve)return false;
        if(T[set_name].penalty<T[b.set_name].penalty)return true;
        if(T[set_name].penalty>T[b.set_name].penalty)return false;
        for(int i = T[set_name].problem_solve;i>=1;i--){
            if(T[set_name].AC_time[i]<T[b.set_name].AC_time[i])return true;
            if(T[set_name].AC_time[i]>T[b.set_name].AC_time[i])return false;
        }
        if(set_name<b.set_name)return true;
        if(set_name>b.set_name)return false;
    }
}str_teamname;
compare newteam;
set<compare>allcompareteam;//放入set中排序
set<compare>frezcompareteam;
//set<compare>::iterator it_set;//注意解引用
int problemnum,minnum=1e9;
void add_team();
void start();
void submit();
void flush();
void flush1();
void freeze();
void query_rank();
void query_submission();
void scroll();
void print();
bool check(string a);
void end();
int main() {
    //freopen("error.in","r",stdin);
    //freopen("error.txt","w",stdout);
    while(true){
        string str ;
        cin>>str;
        if(str=="ADDTEAM"){
            add_team();
        }
        if(str=="START"){
            start();
        }
        if(str=="SUBMIT"){
            submit();
        }
        if(str=="FLUSH"){
            flush();
        }
        if(str=="FREEZE"){
            freeze();
        }
        if(str=="QUERY_RANKING"){
            query_rank();
        }
        if(str=="QUERY_SUBMISSION"){
            query_submission();
        }
        if(str=="SCROLL"){
            scroll();
        }
        if(str=="END"){
            end();
            break;
        }
    }
    return 0;
}
void add_team(){
    team TEAM;
    cin>>TEAM.name;
    if(ifstart==1) {cout<<"[Error]Add failed: competition has started."<<endl;return;}
    else
    {
        if(T.count(TEAM.name)) {
            //T[TEAM.name] ;
            //cout<< "[Info]Add successfully."<<endl;
            //return;
            cout<<"[Error]Add failed: duplicated team name."<<endl;
            return;
        }
        else {
            T[TEAM.name];
            cout<<"[Info]Add successfully."<<endl;
        }
    }
    //compare compareteam;
    str_teamname.set_name=TEAM.name;//暂存（有名字的
    allcompareteam.insert(str_teamname);//放入set中排序
    frezcompareteam.insert(str_teamname);
}
void start(){
    if(ifstart==1)
    {
        string a1;
        cin>>a1>>a1>>a1>>a1;
        cout<<"[Error]Start failed: competition has started."<<endl;
        return;
    }
    else {
        ifstart = true;
        string a,b;
        int duration_time;
        cin>>a>>duration_time>>b>>problemnum;
        cout<<"[Info]Competition starts."<<endl;
    }
}
void submit() {
    string a, teamname, status;
    char problem;
    int time;
    cin >> problem >> a >> teamname >> a >> status >> a >> time;
    if(frozen){
        //T[teamname].freeze=true;
        compare tmp_team;
        tmp_team.set_name=teamname;
        frezcompareteam.insert(tmp_team);
    }
    if (status == "Accepted" && T[teamname].state[problem] != "Accepted") {
        T[teamname].all_solved += 1;
        T[teamname].all_penalty += time + T[teamname].all_submit[problem] * 20;
        T[teamname].state[problem] = status;
        T[teamname].problem[status]=problem;
        T[teamname].specific_state[problem][status] = time;
        //T[teamname].MAXPASSTIME = (T[teamname].MAXPASSTIME > time) ? T[teamname].MAXPASSTIME : time;
        T[teamname].MAXPASSTIME.insert(time);
        T[teamname].latest_submission = problem;
        //T[teamname].status_to_problm[status] = problem;
        T[teamname].all_AC_time[T[teamname].all_solved]=time;
        //T[teamname].ALL_AC_TIME.insert(time);
        //T[teamname].problem_solve+=1;//总解决题数
        //T[teamname].penalty+=time+T[teamname].submit_times[problem]*20;//submit_times记录所有提交失败次数；
        if (!frozen) {
            T[teamname].frez_state[problem]=status;
            T[teamname].submit_times[problem]=T[teamname].all_submit[problem];
            T[teamname].penalty = T[teamname].all_penalty;
            T[teamname].problem_solve = T[teamname].all_solved;
            T[teamname].AC_time[T[teamname].all_solved]=time;
            //T[teamname].AC_TIME.insert(time);
        }
        return;
    }
    if (T[teamname].state[problem] != "Accepted")
    {
        T[teamname].latest_submission = problem;
        T[teamname].state[problem] = status;
        T[teamname].problem[status]=problem;
        T[teamname].specific_state[problem][status] = time;
        T[teamname].all_submit[problem] += 1;
        if (!frozen) {
            T[teamname].frez_state[problem]=status;
            T[teamname].submit_times = T[teamname].all_submit;
            T[teamname].penalty = T[teamname].all_penalty;
            T[teamname].problem_solve = T[teamname].all_solved;
        }
        return;
    }
}
void flush(){
    allcompareteam.clear();
    unordered_map<string,team>::iterator it1;
    for(it1 = T.begin();it1 != T.end() ; it1++)
    {
        newteam.set_name = it1->first;
        allcompareteam.insert(newteam);
    }
    int cnt=1;
    set<compare>::iterator it;
    for(it = allcompareteam.begin() ; it != allcompareteam.end() ; it++){
        T[it->set_name].rank=cnt;
        cnt++;
    }
    cout<<"[Info]Flush scoreboard."<<endl;
}
void flush1(){
    allcompareteam.clear();
    unordered_map<string,team>::iterator it1;
    for(it1 = T.begin();it1 != T.end() ; it1++)
    {
        newteam.set_name = it1->first;
        allcompareteam.insert(newteam);
    }
    int cnt=1;
    set<compare>::iterator it;
    for(it = allcompareteam.begin() ; it != allcompareteam.end() ; it++){
        T[it->set_name].rank=cnt;
        cnt++;
    }
    //cout<<"[Info]Flush scoreboard."<<endl;
}
void freeze(){
    if(frozen){
        cout<<"[Error]Freeze failed: scoreboard has been frozen."<<endl;
        return;
    }
    else {
        cout<<"[Info]Freeze scoreboard."<<endl;
        frozen=1;
    }
    for(auto it=allcompareteam.begin();it!=allcompareteam.end();it++){
        auto thisteam=*it;
        auto thatteam=T[thisteam.set_name];
        for(int i = 1;i<=problemnum;i++){
            if(thatteam.state['A'+i-1]=="Accepted"||thatteam.all_submit['A'+i-1]==0)thatteam.freeze['A'+i-1]=false;
            else thatteam.freeze['A'+i-1]=true;
        }
    }
}
void query_rank(){
    //flush1();
    int count=1;
    for(auto it=allcompareteam.begin();it!=allcompareteam.end();it++ ){
        T[it->set_name].rank=count;
        count++;
    }
    string thisname;
    cin>>thisname;
    if(!T.count(thisname)){
        cout<<"[Error]Query ranking failed: cannot find the team."<<endl;
    }else {
        cout << "[Info]Complete query ranking." << endl;
        if (frozen)
            cout << "[Warning]Scoreboard is frozen. The ranking may be inaccurate until it were scrolled." << endl;
        cout << thisname << " NOW AT RANKING " << T[thisname].rank << endl;
    }
}
void query_submission(){
    string query_question,query_status,a,teamname;
    string problem_status;int submit_time;
    char problem;
    bool query_all_question=0,query_all_status=0;
    bool all_problem=0,all_state=0;
    cin>>teamname>>a>>query_question>>a>>query_status;
    if(!T.count(teamname)){cout<<"[Error]Query submission failed: cannot find the team."<<endl;return;}
    if(query_question[query_question.length()-4]=='=')all_problem=1;
    if(query_status[query_status.length()-4]=='=')all_state=1;
    //if(!T.count(teamname)){cout<< "[Error]Query submission failed: cannot find the team."<<endl;return ;}
    if(all_problem&&all_state)
    {
        if(T[teamname].latest_submission!=0){
            problem = T[teamname].latest_submission;
            problem_status=T[teamname].state[problem];
            submit_time=T[teamname].specific_state[problem][problem_status];
            cout<<"[Info]Complete query submission."<<endl;
            cout<<teamname<<" "<<problem<<" "<<problem_status<<" "<<submit_time<<endl;
            return;
        }
        else {cout<<"[Info]Complete query submission."<<endl;
            cout<<"Cannot find any submission."<<endl;return;}
    }else {
        if (all_state) {
            problem = query_question[query_question.length() - 1];
            if (!T[teamname].submit_times.count(problem)) {
                cout<<"[Info]Complete query submission."<<endl;
                cout << "Cannot find any submission." << endl;
                return;
            }
            problem_status = T[teamname].state[problem];
            submit_time = T[teamname].specific_state[problem][problem_status];
            cout << "[Info]Complete query submission." << endl;
            cout << teamname << " " << problem << " " << problem_status << " " << submit_time << endl;
            return;
        }
        // /*if(query_all_question){
        //   if(query_status=="Wrong_Answer") {
        //      problem = T[teamname].latest_WA;
        //      if (!problem) {
        //         cout << "Cannot find any submission." << endl;
        //         return;
        //     }
        //     problem_status = query_status;
        //    submit_time = T[teamname].specific_state[problem][problem_status];
        //   cout << "[Info]Complete query submission." << endl;
        //   cout << teamname << " " << problem << " " << submit_time << endl;
        //     return;
        //  }
        //  if(query_status=="Runtime_Error") {
        //     problem = T[teamname].latest_RE;
        //     if (!problem) {
        //         cout << "Cannot find any submission." << endl;
        //        return;
        //    }
        //  problem_status = query_status;
        //   submit_time = T[teamname].specific_state[problem][problem_status];
        //     cout << "[Info]Complete query submission." << endl;
        //     cout << teamname << " " << problem << " " << submit_time << endl;
        //     return;
        //  }
        // if(query_status=="Time_Limit_Exceed") {
        //      problem = T[teamname].latest_TLE;
        //      if (!problem) {
        //        cout << "Cannot find any submission." << endl;
        //         return;
        //     }
        //    problem_status = query_status;
        //     submit_time = T[teamname].specific_state[problem][problem_status];
        //     cout << "[Info]Complete query submission." << endl;
        //     cout << teamname << " " << problem << " " << submit_time << endl;
        //    return;
        //  }
        if (all_problem) {
            problem_status = query_status.substr(7);
            if (!T[teamname].problem.count(problem_status)) {
                cout << "[Info]Complete query submission." << endl;//zijijia
                cout << "Cannot find any submission." << endl;
                return;
            }
            problem = T[teamname].problem[problem_status];
            submit_time = T[teamname].specific_state[problem][problem_status];
            cout << "[Info]Complete query submission." << endl;
            cout << teamname << " " << problem << " " << problem_status << " " << submit_time << endl;
            return;
        }
        problem = query_question[query_question.length() - 1];
        problem_status = query_status.substr(7);
        if (T[teamname].specific_state.count(problem) && T[teamname].specific_state[problem].count(problem_status)) {
            submit_time = T[teamname].specific_state[problem][problem_status];
            cout << "[Info]Complete query submission." << endl;
            cout << teamname << " " << problem << " " << problem_status << " " << submit_time << endl;
            return;
        } else {
            cout << "[Info]Complete query submission." << endl;
            cout << "Cannot find any submission." << endl;
        }
    }
}
void print(){
    set<compare>::iterator it;
    if(frozen) {
        for (it = allcompareteam.begin(); it != allcompareteam.end(); it++) {
            if(it->set_name==" ")continue;
            auto thisteam = *it;
            auto thatteam = T[thisteam.set_name];
            cout << thisteam.set_name << " " << thatteam.rank << " " << thatteam.problem_solve << " "<< thatteam.penalty<< " ";
            for (int i = 1; i <= problemnum; i++) {
                char pro = 'A' - 1 + i;
                int x = thatteam.submit_times[pro];//提交次数(only false times)
                int y = thatteam.all_submit[pro] - x;//冻结期提交次数
                if(x==0&&y==0&&thatteam.state[pro]=="Accepted"&&thatteam.frez_state[pro]!="Accepted"){
                    cout<<"0/"<<1<<" ";continue;
                }
                if ( thatteam.frez_state[pro] == "Accepted")//未冻结ac数
                {
                    if (x == 0)cout << "+";
                    else cout << "+" << x;//输出的是错误次数
                    cout << " ";
                    continue;
                }
                if (thatteam.frez_state[pro] != "Accepted" && y == 0 &&
                    thatteam.state[pro] != "Accepted")//不冻不ac
                {
                    if (x == 0)cout << ".";
                    else cout << "-" << x;
                    cout << " ";
                    continue;
                }
                if (thatteam.frez_state[pro]!="Accepted" && thatteam.state[pro] == "Accepted") {
                    if (x == 0 && y != 0)cout << "0/" << y + 1;
                    else cout << "-" << x << "/" << y + 1;
                    cout << " ";
                    continue;
                }
                if (thatteam.state[pro] != "Accepted") {
                    if (x == 0 && y != 0)cout << "0/" << y;
                    else cout << "-" << x << "/" << y;
                    cout << " ";
                    continue;
                }
            }
            cout << endl;
        }
        return;
    }
    if(!frozen)
    {
        for(it=allcompareteam.begin();it!=allcompareteam.end();it++)
        {
            if(it->set_name==" ")continue;
            cout<<it->set_name<<" "<<T[it->set_name].rank<<" "<<T[it->set_name].problem_solve<<" "<<T[it->set_name].penalty<<" ";
            team thatteam=T[it->set_name];
            for(int i=1;i<=problemnum;i++)
            {
                char pro = 'A'-1+i;
                int x = thatteam.all_submit[pro];
                if(thatteam.state[pro]=="Accepted"&&x==0)
                {
                    cout<<"+"<<" ";
                    continue;
                }
                if(thatteam.state[pro]=="Accepted"&&x!=0)
                {
                    cout<<"+"<<x<<" ";
                    continue;
                }
                if(!thatteam.state.count(pro)||x==0)
                {
                    cout<<"."<<" ";
                    continue;
                }
                // if(thatteam.state.count(pro)&&x==1)
                //{
                //  cout<<"-"<<" ";
                //continue;
                //}
                if(thatteam.state.count(pro)&&x>=1)
                {
                    cout<<"-"<<x<<" ";
                    continue;
                }
            }
            cout<<endl;
        }
    }
}
void scroll()
{
    if (!frozen)
    {
        cout << "[Error]Scroll failed: scoreboard has not been frozen." << endl;
    } else
    {
        cout << "[Info]Scroll scoreboard." << endl;
        flush1();
        print();
        //for (auto it = allcompareteam.end(); it != allcompareteam.begin(); it--) {
        //it--;
        //while (check(it->set_name)) {
        //char pro='A'+minnum-1;//检验最底行行有没有freeze的题目；并记录,,解冻最小题,并重新算分；
        // T[it->set_name].freeze[pro]=false;
        // T[it->set_name].submit_times[pro]=T[it->set_name].all_submit[pro];
        // if(T[it->set_name].state['A'+minnum+1]=="Accepted")
        // {
        //     T[it->set_name].problem_solve++;
        //     //T[it->set_name].all_solved++;
        //     T[it->set_name].penalty+=(T[it->set_name].all_submit[pro]-T[it->set_name].submit_times[pro])*20+T[it->set_name].all_AC_time[pro];
        //     T[it->set_name].AC_time[minnum]=T[it->set_name].all_AC_time[minnum];
        //  }
        // flush1();
        // cout<<it->set_name<<" ";
        // it++;
        // cout<<it->set_name<<" ";
        //it--;
        // cout<<T[it->set_name].problem_solve<<" "<<T[it->set_name].penalty<<endl;
        set<compare>::iterator  it=frezcompareteam.end();
        set<compare>::iterator ptr1;
        set<compare>::iterator ptr2;
        //it = frezcompareteam.end();
        do
        {
            it--;
            string name = it->set_name;
            team that_team = T[it->set_name];
            if(that_team.all_solved!=that_team.problem_solve)
            {
                for (int i = 1; i <= problemnum; i++)
                {
                    char pro = 'A' + i - 1;
                    int tmp_rank = 0;
                    //int tmp;
                    compare enermy;
                    //T[name].freeze[pro]=false;
                    //tmp=T[name].submit_times[pro];//自加
                    //T[name].submit_times[pro]=T[name].all_submit[pro];
                    if (that_team.state[pro]=="Accepted"&&that_team.frez_state[pro]!="Accepted")
                    {
                        //string state = T[name].state[pro];
                        str_teamname.set_name=name;
                        allcompareteam.erase(str_teamname);
                        allcompareteam.insert(str_teamname);
                        //flush1();
                        ptr1=allcompareteam.find(str_teamname);
                        ptr2=allcompareteam.upper_bound(*ptr1);
                        T[name].freeze[pro]=0;
                        T[name].problem_solve+=1;
                        T[name].submit_times[pro] = T[name].all_submit[pro];//有问题
                        T[name].penalty+=T[name].submit_times[pro]*20+T[name].specific_state[pro]["Accepted"];//有问题
                        T[name].frez_state[pro]=T[name].state[pro];
                        tmp_rank=T[name].rank;
                        //newteam.set_name=name;
                        //auto ptr1=allcompareteam.find(newteam);
                        //auto ptr2=allcompareteam.upper_bound(*ptr1);
                        enermy.set_name=ptr2->set_name;
                        //allcompareteam.erase(*ptr1);
                        //flush1();
                        allcompareteam.erase(str_teamname);
                        allcompareteam.insert(str_teamname);
                        ptr1 = allcompareteam.find(str_teamname);
                        ptr2 = allcompareteam.upper_bound(*ptr1);
                        if(enermy.set_name!=ptr2->set_name)
                        {
                            cout<<name<<" "<<ptr2->set_name<<" "<<T[name].problem_solve<<" "<<T[name].penalty<<endl;
                        }
                    }
                }
                for(int i = 1;i<=26;i++)T[name].AC_time[i]=T[name].all_AC_time[i];
            }
        } while (it != frezcompareteam.begin());
        frozen=false;
        flush1();
        print();
        frezcompareteam.clear();
        unordered_map<string,team>::iterator it1;
        for(it1=T.begin();it1!=T.end();it1++)
        {
            str_teamname.set_name=it1->first;
            frezcompareteam.insert(str_teamname);
        }
        int counter = 1 ;
        set<compare>::iterator point;
        for(point=allcompareteam.begin();point!=allcompareteam.end();point++)
        {
            T[point->set_name].rank=counter;
            counter++;
        }
        unordered_map<string,team>::iterator  point2;
        for(point2 = T.begin();point2!=T.end();point2++)
        {
            str_teamname.set_name=point2->first;
            frezcompareteam.insert(str_teamname);
        }
    }
    //flush1();
    return;
}
//bool check( string a ){
//  bool flag=false;
//for(int i = 1;i<=problemnum;i++){
//  flag=false;
//if(T[a].freeze['A'+i-1]==1){
//  minnum=min(minnum,i);
//flag=true;
//}
//}
//return flag;
//}
void end(){
    cout<<"[Info]Competition ends."<<endl;
}*/
#include <iostream>
#include <unordered_map>
#include <set>
#include <vector>
#include <algorithm>
using namespace std;
int begin_time=0,problemnum=0,teamnum=0,problemcnt=0;
bool ifstart=0,iffreeze=0;
string* team_rank;
struct key_team{
    int ac_num,Penalty,ac_time[27]={0};
    string teamname;
    bool operator <(const key_team& b) const
    {
        if (ac_num>b.ac_num) return 1;
        if (ac_num<b.ac_num) return 0;
        if (Penalty<b.Penalty) return 1;
        if (Penalty>b.Penalty) return 0;
        for (int i=ac_num;i>=1;--i) {
            if (ac_time[i] < b.ac_time[i]) return 1;
            if (ac_time[i] > b.ac_time[i]) return 0;
        }
        if (teamname<b.teamname) return 1;
        return 0;
    }
};
struct team{
    string name=" ";
    int all=0,ac=0,wa=0,tle=0,re=0,pro[27]={0},acpro[27]={0},wapro[27]={0},tlepro[27]={0},repro[27]={0};
    int freeze=0,accepted=0,rank=0,PENALTY=0,ac_time[27]={0},submit[27]={0},frez_submit[27]={0};
    bool have_ac[27]={0},have_frozen[27]={0};
    vector<int>frez_pro[27];
};
struct problem{
    string team_name,status,problem_name;
    int sub_time;
};
unordered_map<string,team> map_name_team;
unordered_map<string,key_team> map_name_key;
vector<problem> pro;
vector<string> team_name_v;
set<key_team> set_team;//排序用
set<key_team> set_fteam;
void init();
void Start();
void AddTeam();
void end();
void Submit();
void Flush(bool flag);
void Freeze();
void Scroll();
void Query_rank();
void Query_submission();
void insert(string& status,unordered_map<string,team>::iterator tmp,int &number,int &problem_num);
int main(){
    init();
    freopen("bigger.in","r",stdin);
    freopen("bigger.txt","w",stdout);
    while(true){
        string str;
        cin>>str;
        if(str=="ADDTEAM"){
            AddTeam();
        }
        if(str=="START"){
            Start();
        }
        if(str=="SUBMIT"){
            Submit();
        }
        if(str=="FLUSH"){
            Flush(1);
        }
        if(str=="FREEZE"){
            Freeze();
        }
        if(str=="QUERY_RANKING"){
            Query_rank();
        }
        if(str=="QUERY_SUBMISSION"){
            Query_submission();
        }
        if(str=="SCROLL"){
            Scroll();
        }
        if(str=="END"){
            end();
            break;
        }
    }
    return 0;
}
void init(){
    map_name_key.clear();
    map_name_team.clear();
    pro.clear();
    pro.push_back({" "," "," ",0});
    team_name_v.clear();
    team_name_v.emplace_back(" ");
}
void Start(){
    int begin_time_tmp=0,problem_count_tmp=0;
    string s;
    cin>>s>>begin_time_tmp>>s>>problem_count_tmp;
    if(!ifstart)
    {
        begin_time=begin_time_tmp;
        problemcnt=problem_count_tmp;
        team_rank=new string [teamnum+1];//pengzidong shabi
        ifstart=true;
        Flush(0);
        cout<<"[Info]Competition starts."<<endl;
    }
    else cout<<"[Error]Start failed: competition has started."<<endl;
}
void AddTeam(){
    if(ifstart){cout<<"[Error]Add failed: competition has started."<<endl;}
    else {
        string s;
        cin>>s;
        if(map_name_team.count(s)){
            cout<<"[Error]Add failed: duplicated team name."<<endl;return;
        }
        team_name_v.push_back(s);
        teamnum++;
        team tmp;
        tmp.name=s;
        map_name_team[s]=tmp;
        key_team tmp2;
        tmp2.ac_num=0;
        tmp2.Penalty=0;
        tmp2.teamname=s;
        map_name_key[s]=tmp2;
        set_team.insert(tmp2);
        cout<<"[Info]Add successfully."<<endl;
    }
}
void Submit(){
    string useless;
    string problemname,teamname,status;
    int submittime;
    cin>>problemname>>useless>>teamname>>useless>>status>>useless>>submittime;
    int number=problemname[0]-'A';//找到是第几题
    auto tmp_team=map_name_team.find(teamname);
    auto tmp_key=map_name_key.find(teamname);
    problem tmp_problem;
    tmp_problem.team_name=teamname;
    tmp_problem.problem_name=problemname;
    tmp_problem.sub_time=submittime;
    tmp_problem.status=status;
    pro.emplace_back(tmp_problem);
    //pro.push_back(tmp_problem);
    problemnum++;
    if(!iffreeze){
        if(status=="Accepted"){
            if(!tmp_team->second.have_ac[number]){
                set_team.erase(tmp_key->second);
                tmp_team->second.accepted++;
                tmp_team->second.PENALTY+=submittime+20*tmp_team->second.submit[number];
                tmp_team->second.ac_time[tmp_team->second.accepted]=submittime;
                tmp_key->second.ac_time[tmp_team->second.accepted]=submittime;
                tmp_key->second.ac_num=tmp_team->second.accepted;
                tmp_key->second.Penalty=tmp_team->second.PENALTY;
                tmp_team->second.have_ac[number]=1;
                set_team.insert(tmp_key->second);
            }
        }else{
            if(!tmp_team->second.have_ac[number]){
                tmp_team->second.submit[number]++;
            }
        }
    }else{
        tmp_team->second.frez_pro[number].push_back(problemnum);
        if(!tmp_team->second.have_frozen[number]){
            tmp_team->second.have_frozen[number]=1;
            ++tmp_team->second.freeze;
        }
        tmp_team->second.frez_submit[number]++;
    }
    insert(status,tmp_team,number,problemnum);
}
void insert(string& status,unordered_map<string,team>::iterator tmp,int &number,int &problem_num){
    if(status=="Accepted"){
        tmp->second.ac=problem_num;
        //tmp->second.acpro[number]=problem_num;
    }
    else if(status=="Wrong_Answer"){
        tmp->second.wa=problem_num;
        //tmp->second.wapro[number]=problem_num;
    }
    else if(status=="Time_Limit_Exceed"){
        tmp->second.tle=problem_num;
        //tmp->second.tlepro[number]=problem_num;
    }
    else if(status=="Runtime_Error"){
        tmp->second.re=problem_num;
        //tmp->second.repro[number]=problem_num;
    }
    tmp->second.pro[number]=problem_num;
    //tmp->second.all=problem_num;
    if (status == "Accepted") {
        tmp->second.acpro[number]=problem_num;
    } else if (status == "Wrong_Answer") {
        tmp->second.wapro[number]=problem_num;
    } else if (status == "Runtime_Error") {
        tmp->second.repro[number]=problem_num;
    } else if (status == "Time_Limit_Exceed") {
        tmp->second.tlepro[number]=problem_num;
    }
    tmp->second.all=problem_num;
}
void Flush(bool flag){
    int rank=1;
    string teamname;
    for(const auto & it : set_team){//may be false;
        teamname=it.teamname;
        auto tmp_team=map_name_team.find(teamname);
        tmp_team->second.rank=rank;//最后一次rank可能出锅
        team_rank[rank]=it.teamname;
        rank++;
    }
    if(flag)cout<<"[Info]Flush scoreboard."<<endl;
}
void Freeze(){
    if(iffreeze){cout<<"[Error]Freeze failed: scoreboard has been frozen."<<endl;}
    else {
        cout<<"[Info]Freeze scoreboard."<<endl;
        for(int i = 1; i<=teamnum;i++){
            auto tmp=map_name_team.find(team_name_v[i]);
            tmp->second.freeze=0;
            for(int j = 0;j<problemcnt;j++){
                tmp->second.frez_submit[j]=0;
                tmp->second.have_frozen[j]=false;
                tmp->second.frez_pro[j].clear();
            }
        }
        iffreeze=true;
    }
}
void Query_rank(){
    string teamname;cin>>teamname;
    if(!map_name_team.count(teamname)){
        cout<<"[Error]Query ranking failed: cannot find the team."<<endl;
        return;
    }
    cout<<"[Info]Complete query ranking."<<endl;
    auto tmp=map_name_team.find(teamname);
    if(iffreeze)cout<<"[Warning]Scoreboard is frozen. The ranking may be inaccurate until it were scrolled."<<endl;
    cout<<tmp->second.name<<" NOW AT RANKING "<<tmp->second.rank<<endl;
}
void Query_submission(){
    string useless,problem,status=" ",teamname=" ";
    //char problem1[100];
    cin>>teamname>>useless>>problem>>useless>>status;
    if(map_name_team.count(teamname)==0){cout<<"[Error]Query submission failed: cannot find the team."<<endl;return;}
    if(status[status.length()-4]=='='){status="ALL";}
    else status=status.substr(7);
    if(problem[problem.length()-4]=='='){problem="ALL";}
    else problem=problem[problem.length()-1];
    auto tmp_team=map_name_team.find(teamname);
    int ans=0;
    if(status=="ALL"&&problem=="ALL"){
        ans=tmp_team->second.all;
    }else if(status!="ALL"&&problem=="ALL"){
        if(status=="Accepted")
            ans=tmp_team->second.ac;
        else
        if(status=="Runtime_Error")
            ans=tmp_team->second.re;
        else
        if(status=="Time_Limit_Exceed")
            ans=tmp_team->second.tle;
        else
        if(status=="Wrong_Answer")
            ans=tmp_team->second.wa;
    }
    else if(status=="ALL"&&problem!="All"){
        ans=tmp_team->second.pro[problem[0]-'A'];
    }else {
        if (status=="Accepted")
            ans=tmp_team->second.acpro[problem[0]-'A'];
        else
        if (status=="Runtime_Error")
            ans=tmp_team->second.repro[problem[0]-'A'];
        else
        if (status=="Time_Limit_Exceed")
            ans=tmp_team->second.tlepro[problem[0]-'A'];
        else
        if (status=="Wrong_Answer")
            ans=tmp_team->second.wapro[problem[0]-'A'];
    }
    cout<<"[Info]Complete query submission."<<endl;
    if(ans==0)cout<<"Cannot find any submission."<<endl;
    else cout<<pro[ans].team_name<<" "<<pro[ans].problem_name<<" "<<pro[ans].status<<" "<<pro[ans].sub_time<<endl;
}
void Scroll(){
    if (!iffreeze) {
        cout<<"[Error]Scroll failed: scoreboard has not been frozen."<<endl;
        return;
    }
    unordered_map<string,team>::iterator tmp;
    set_fteam.clear();
    iffreeze=false;
    int cnt=0;Flush(0);
    cout<<"[Info]Scroll scoreboard."<<endl;
    for(int i = 1 ; i <= teamnum ; i++){//第一遍输出榜单
        tmp=map_name_team.find(team_rank[i]);//暂存排名的队伍
        if(tmp->second.freeze){
            auto now = map_name_key.find(team_rank[i]);
            set_fteam.insert(now->second);
            cnt++;//有多少个冻结题目的队伍
        }
        cout<<tmp->second.name<<" "<<i<<" "<<tmp->second.accepted<<' '<<tmp->second.PENALTY;
        for(int j = 0 ;j<problemcnt;j++){
            if(tmp->second.have_ac[j]){
                if(tmp->second.submit[j])cout<<" +"<<tmp->second.submit[j];
                else cout<<" +";
            }else
            if (!tmp->second.have_frozen[j]){
                if (tmp->second.submit[j]) cout<<" -"<<tmp->second.submit[j];
                else cout<<" .";
            }else{
                if (tmp->second.submit[j]) cout<<" -"<<tmp->second.submit[j]<<'/'<<tmp->second.frez_submit[j];
                else cout<<' '<<tmp->second.submit[j]<<'/'<<tmp->second.frez_submit[j];
            }
        }
        cout<<endl;
        }
    while(cnt!=0){//有冻结的队伍
        key_team p=*(set_fteam.rbegin());//最后一个
        auto tmp_key=map_name_key.find(p.teamname);
        auto tmp=map_name_team.find(p.teamname);
        int j = 0;
        for(;j<problemcnt;j++){
            if(tmp->second.have_frozen[j])break;
        }
        tmp->second.have_frozen[j]=0;
        --tmp->second.freeze;//冻结题目数-1
        if(tmp->second.have_ac[j]){
            if (!tmp->second.freeze) {set_fteam.erase(p);cnt--;}//没有冻结题目，cnt为有冻结题目的队伍数
        }else {
            bool flag_ac=false;int sub=0,ac_time;
            for(int i = tmp->second.frez_pro[j].size()-1;i>=0;i--){
                int x=tmp->second.frez_pro[j][i];
                if(pro[x].status=="Accepted"){
                    flag_ac=true;
                    sub=0;
                    ac_time=pro[x].sub_time;
                }else sub++;
            }
            if(flag_ac){
                tmp->second.submit[j]+=sub;
                tmp->second.accepted++;
                tmp->second.ac_time[tmp->second.accepted]=ac_time;
                tmp->second.have_ac[j]=1;
                tmp->second.PENALTY+=20*tmp->second.submit[j]+ac_time;
                sort(tmp->second.ac_time+1,tmp->second.ac_time+tmp->second.accepted+1);
                tmp_key->second.ac_num++;
                tmp_key->second.ac_time[tmp->second.accepted]=ac_time;
                tmp_key->second.Penalty+=20*tmp->second.submit[j]+ac_time;
                sort(tmp_key->second.ac_time+1,tmp_key->second.ac_time+tmp->second.accepted+1);
                set_fteam.erase(p);
                if (tmp->second.freeze){
                    set_fteam.insert(tmp_key->second);
                } else cnt--;
                auto it=set_team.lower_bound(tmp_key->second);
                if (p.teamname != (*it).teamname)
                    cout<<tmp_key->second.teamname<<' '<<(*it).teamname<<' '<<tmp_key->second.ac_num<<' '<<tmp_key->second.Penalty<<endl;
                set_team.erase(p);
                set_team.insert(tmp_key->second);
            }else {
                tmp->second.submit[j]+=sub;
                if(!tmp->second.freeze){
                    cnt--;
                    set_fteam.erase(p);
                }
            }
        }
    }
    Flush(0);
    for (int i=1;i<=teamnum;++i) {
        tmp = map_name_team.find(team_rank[i]);
        if (tmp->second.freeze) {
            auto now = map_name_key.find(team_rank[i]);
            set_fteam.insert(now->second);
            ++cnt;
        }
        cout << tmp->second.name << ' ' << i << ' ' << tmp->second.accepted << ' ' << tmp->second.PENALTY;
        for (int j = 0; j < problemcnt; ++j) {
            if (tmp->second.have_ac[j]) {
                if (tmp->second.submit[j]) cout << " +" << tmp->second.submit[j];
                else cout << " +";
            } else if (!tmp->second.have_frozen[j]) {
                if (tmp->second.submit[j]) cout << " -" << tmp->second.submit[j];
                else cout << " .";
            } else {
                if (tmp->second.submit[j]) cout << " -" << tmp->second.submit[j] << '/' << tmp->second.frez_submit[j];
                else cout << ' ' << tmp->second.submit[j] << '/' << tmp->second.frez_submit[j];
            }
        }
        cout << endl;
    }
}
void end(){
    cout<<"[Info]Competition ends."<<endl;
}