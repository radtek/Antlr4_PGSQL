#include "MySqlAntlr_Parser.h"
#include  <vector>
#include <string>
typedef vector<string> * pvector;
void typechenge(vector<string> &v,vector<string> &function,int &chenge,string temp,string tableback="");

//这两个是输出function的函数  因为有"()"标识
void print(int count,string type,string &temp){
    if(temp=="")
        return ;
    while(count--)
        cout<<"\t";
        cout<<type<<temp<<endl;;
        temp="";
}

void vectorback(vector<string> &v){
	int i=0,count=0;
    string temp,name="FUNC_name: ",value="Parameter: ";
    while(i!=v.size())
    {
        string c=v[i];
        if(c=="("){
            print(count,name,temp);
            count++;
        }else
        if(c==")"){
            print(count,value,temp);
            count--;
        }else
        if(c==","){
            print(count,value,temp);
        }
        else
            temp+=c;
        i++;
	}
	v.clear();
}


//插入函数，每一次插入都会保留，并且在第二次插入判断是否早已存在，true然后pop
void vectorpush(vector<string> &v, string s) {

	if(find(v.begin(), v.end()-1, v.back()) != v.end()-1)
		v.pop_back();
	v.push_back(s);
		
}

//发现函数判断某个string是否在已定义的过滤字符串中(vector<string>nodate 过滤)
bool stringnofind(vector<string>nodate, string str) {
	vector<string>::iterator iter;
	for (iter = nodate.begin(); iter != nodate.end(); iter++)
	{
		if (*iter == str)
			return false;
	}
	return true;
}

//输出函数
void vectorprint(vector<vector<string>> &sqlnames,vector<vector<string>> &tablenames, vector<vector<string>> &columnnames,vector<vector<string>> &functions) {
	unsigned int i = 0,j=0;
	unsigned int count=tablenames.size();
	for(i=0;i<count;i++){
		cout << "\nsqlname\n";
		for (j = 0; j < sqlnames[i].size(); j++)
			cout << sqlnames[i][j] << " ";
		cout << "\ntablename\n";
		for (j = 0; j < tablenames[i].size(); j++)
			cout << tablenames[i][j] << " ";
		cout << "\ncolumnname\n";
		for (j = 0; j < columnnames[i].size(); j++)
			cout << columnnames[i][j] << " ";
		cout << "\nfunction\n";
		for (j = 0; j < functions[i].size(); j++)
            cout << functions[i][j] << " ";
        cout << "\n///////////////////////////////////////////////\n";
	}
}


//若遇到函数却无法识别而将其识别成表或者列时，通过"()"标识将其分离出来

void typechenge(vector<string> &v,vector<string> &function,int &chenge,string temp,string tableback){
	if(chenge==0){
        if(temp==",")
            v.push_back(temp);// up is usually
        else if(temp=="("){
            chenge++;
			function.push_back(v.back());
			v.pop_back();
            function.push_back(temp);
		}
		else if(temp!=")")
		    vectorpush(v,tableback+temp);
    }
	else{
	    if (temp == "("){
		    chenge ++;
		    function.push_back(temp);
	    }
	    else if (temp == ")") {
		    chenge --;
		    function.push_back(temp);
        }
        else  function.push_back(temp);
		}
}

int ANTLR_Parse(char *sql_name) {
	
		ANTLRInputStream input(sql_name);
		MySqlLexer lexer(&input);
		CommonTokenStream tokens(&lexer);

		MySqlParser parser(&tokens);
		tree::ParseTree *t = parser.root();
		std::cout << t->toStringTree(&parser) << std::endl << std::endl;
		const std::vector<std::string> ruleNames = parser.getRuleNames();
		tree::Trees *ptfff;
		std::string temp;
		cout<<temp<<endl;
		if (t->children.empty()) {
			cout << "input is empty()\n";
			return -1;
		}
	
		std::stack<size_t> stack;
		size_t childIndex = 0;
		tree::ParseTree *run = t;
	
		bool typeindex = true;
		
		string query_specification[] = { "SELECT","DROP","INSERT" };
		string tableback, tempname, son,tempback;
	
		string word[] = { "WHERE","AND","OR"};
        vector<string>nodate (word, word + 3);

        int findin = 0,chenge=0,astemp=0,key=0,key1=0;
		vector<vector<string>>tablenames,columnnames,functions,sqlnames;
		vector<string> vectornull;
		tablenames.push_back(vectornull);
		columnnames.push_back(vectornull);
		functions.push_back(vectornull);
		sqlnames.push_back(vectornull);
		pvector tablename=&tablenames[0],columnname=&columnnames[0],function=&functions[0],sqlname=&sqlnames[0];
		string SqlType;
		while (childIndex < run->children.size()) {
			tree::ParseTree*child = run->children[childIndex];
			temp = antlrcpp::escapeWhitespace(ptfff->getNodeText(child, ruleNames), false);
			if (!child->children.empty()) {
				stack.push(childIndex);
				run = child;
                childIndex = 0;
                if (chenge<=0){
                    chenge=0;
					tempname = temp;
				  }
			}
			else {
				// if (chenge<=0){
                //     chenge=0;
				// 	//tempname = antlrcpp::escapeWhitespace(ptfff->getNodeText(run->children[0]->parent, ruleNames), false);
				// 	tempname=antlrcpp::escapeWhitespace(ptfff->getNodeText(child->parent, ruleNames), false);
				//   }
                //tempname = antlrcpp::escapeWhitespace(ptfff->getNodeText(run->children[0]->parent, ruleNames), false);
                // tempname=antlrcpp::escapeWhitespace(ptfff->getNodeText(child->parent, ruleNames), false);
                tempback=temp;
				if(temp=="AS")
					astemp=-1;
				else
				if(astemp==-1)
					astemp=1;
                else //过滤as
                if (temp == "IN" )
                    findin++;
                else
                if (findin) {
                    if (temp == "(")
                        findin++;
                    if (temp == ")") {
                        findin--;
                        if (findin == 1)
                            findin = 0;
                    }
				}else//过滤in
				if(temp=="FROM"){
					int tablesize=(*tablename).size();
					int i,j,size;
					for(i=0;i<tablesize;i++){
						j=(*tablename)[i].find('.',0);
						if(j!=-1||(*tablename)[i]!="*"||(*tablename)[i]!="."){
							(*columnname).push_back((*tablename)[i]);
							(*tablename)[i]="";
						}
						i++;
					}
				}else
                if(temp==")"&&chenge<=0&&key>0){
					key--;
					tablename=&tablenames[key];
					columnname=&columnnames[key];
					function=&functions[key];
					sqlname=&sqlnames[key];
                }else
				if (stringnofind(nodate, temp)) {
					if (tempname == "query_specification") {
						(*sqlname).push_back(temp);
					}else
	
					if (tempname == "simple_id") {
						typechenge(*tablename,*function, chenge,temp);
						tableback=temp;
					}else
	
					if (tempname == "dot_ext_id") {
						typechenge(*columnname, *function,chenge,temp,tableback);
					}else
	
					if (tempname == "aggregate_windowed_function") {
						(*function).push_back(temp);
					}
					else
					if (tempname == "expression_atom") {
						tablenames.push_back(vectornull);
						columnnames.push_back(vectornull);
						functions.push_back(vectornull);
						sqlnames.push_back(vectornull);
						int len=tablenames.size()-1;
						
						tablename=&tablenames[len];
						columnname=&columnnames[len];
						function=&functions[len];
						sqlname=&sqlnames[len];
						key++;
					}
				} 
				else ;
				while (++childIndex == run->children.size()) {
	
					if (stack.size() > 0) {
						childIndex = stack.top();
						stack.pop();
						run = run->parent;
					}
					else {
						break;
					}
				}
			}
	  }
	  vectorprint(sqlnames,tablenames, columnnames,functions);
	return 1;
	}