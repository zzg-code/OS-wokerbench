#include "pstree.h"

std::string PATH="/proc/";
std::string Version="1.0.0";

struct TreeNode{
    std::string Process_Name;
    pid_t num{};
    pid_t father{};
    std::vector<TreeNode*> Children;
    TreeNode()= default;
    TreeNode(std::string Process_Name,pid_t num, pid_t father):Process_Name(std::move(Process_Name)),num(num),father(father){}
};

int ReadArgs(int argc,char* argv[]){
    int args=0;
    for(int i=1;i<argc;i++)
    {
        if(argv[i][1]=='p')
        {
            args |=1;
        }
        if(argv[i][1]=='n')
        {
            args |=1<<1;
        }
        if(argv[i][1]=='V')
        {
            args |=1<<2;
        }
    }
    return args;
}

bool isdigitStr(std::string &number){
    for(auto ch:number)
    {
        if(!isdigit(ch))return false;
    }
    return true;
}

std::vector<std::string> ReadFileName(std::string& Path){
    std::vector<std::string> res;
    DIR* dir=opendir(Path.c_str());
    assert(dir);
    struct dirent *entry= nullptr;
    while((entry= readdir(dir)))
    {
        std::string filename;
        if(entry->d_type==DT_DIR&&!(filename=entry->d_name).empty()&&isdigitStr(filename))
        {
            res.push_back(filename);
            filename.clear();
        }
    }
    closedir(dir);
    return res;
}

std::string delete_nop(const std::string& str){
    std::string  res;
    for(auto ch:str)
    {
        if(ch!='\t'&&ch!=' ')
        {
            res.push_back(ch);
        }
    }
    return res;
}

TreeNode* Creat_Tree(std::vector<std::string>& FileNameArr){
    assert(!FileNameArr.empty());
    TreeNode* root= nullptr;
    std::unordered_map<pid_t,TreeNode*> mp;
//    std::sort(FileNameArr.begin(),FileNameArr.end(),[](const std::string& a,const std::string& b){
//       return std::stoi(a)<std::stoi(b);
//    });

    for(auto elem:FileNameArr)
    {
        std::string FileName=PATH+elem+"/status";
        std::ifstream file(FileName,std::ios::in);
        if(!file.is_open()) continue;

        std::string Process_name,pre,str;
        pid_t num,pnum;
        int count=0;
        while(getline(file,str)&&count<3)
        {
            int pos=str.find(':');
            pre=str.substr(0,pos);
            if(pre=="Name")
            {
                Process_name= delete_nop(str.substr(pos+1,str.length()-pos-1));
                count++;
            }
            if(pre=="Pid")
            {
                num=std::stoi(delete_nop(str.substr(pos+1,str.length()-pos-1)));
                count++;
            }
            if(pre=="PPid")
            {
                pnum=std::stoi(delete_nop(str.substr(pos+1,str.length()-pos-1)));
                count++;
            }
        }
        if(num==1||mp.find(pnum)!=mp.end())
        {
            TreeNode* node=new TreeNode(Process_name,num,pnum);
            mp[num]=node;
            if(num!=1)
            {
                mp[pnum]->Children.push_back(node);
            }
            if(num==1) root=node;
        }
    }
    assert(root);
    return root;
}

void print_tree(TreeNode* root, int ArgHash,const std::string& prefix = "", bool is_last = true,bool is_first=true) {
    //assert(root);
    // 打印当前节点
    if(is_first){
        std::cout<< root->Process_Name ;
        if(ArgHash&1)
        {
            std::cout<<"("<<root->num<<")";
        }
        std::cout<<"\n";
        is_first=false;
    }
    else
    {

        std::cout << prefix << (is_last? "└── " : "├── ") << root->Process_Name;
        if(ArgHash&1)
        {
            std::cout<<"("<<root->num<<")";
        }
        std::cout<<"\n";
    }

    // 获取子节点数量
    size_t child_count = root->Children.size();
    if(ArgHash&(1<<1))
    {
        std::sort(root->Children.begin(),root->Children.end(),[](TreeNode* a,TreeNode* b){
            return a->num<b->num;
        });
    }
    // 递归打印子节点
    for (size_t i = 0; i < child_count; ++i) {
        bool last_child = (i == child_count - 1);
        print_tree(root->Children[i], ArgHash,prefix + (is_last ? "\t" : "│\t"), last_child,is_first);
    }
}

int pstree(int argc,char *argv[])
{
    int ArgHash= ReadArgs(argc,argv);
    if(ArgHash&(1<<2))
    {
        std::cout<<"This Process Version is"<<" "<<Version;
        return 0;
    }
    std::vector<std::string> FileNameArr= ReadFileName(PATH);
    TreeNode* root=Creat_Tree(FileNameArr);
    print_tree(root,ArgHash);
    return 0;
}
