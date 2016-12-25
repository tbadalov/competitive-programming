//Solution
#include <bits/stdc++.h>

using namespace std;

class Token {

public:
    char type;
    int val;
};

class TokenStream {

public:
    Token get_token(void);
    void putback(Token t);
    stringstream ss;
private:
    bool full {false};
    Token buffer;
};

int get_expression(void);
int get_primary(void);
int get_number(void);
int get_value(char c);
bool is_op(char c);

TokenStream ts;

map<char, int> values, potential;

int main()
{
    char c; string line;
    //freopen("out.txt", "w", stdout);
    while ( (c = getchar()) != '#' )
    {
        cin.putback(c);
        getline(cin, line);
        line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());
        ts.ss.str(line); ts.ss.clear();

        potential.clear();
        get_expression(); ts.get_token();

        set<char> answers;
        for ( map<char, int>::iterator it = potential.begin(); it != potential.end(); it++ )
            answers.insert(it->first);
        if ( answers.size() == 0 )
        {
            cout << "No Change" << endl;
            continue;
        }

        for ( set<char>::iterator it = answers.begin(); it != answers.end(); it++ )
            cout << (it == answers.begin()? "" : ", ") << *it << " = " << (values[*it] = potential[*it]);
        cout << endl;
    }

    return 0;
}

void TokenStream::putback(Token t){

    ts.full = true; ts.buffer = t;
}

Token TokenStream::get_token(void){

    if ( ts.full )
    {
        ts.full = false;
        return ts.buffer;
    }

    char c;
    if ( !ts.ss || !(ts.ss >> c) )
        return Token{'f'};

    if (  c >= '0' && c <= '9' )
        ts.ss.putback(c);

    return c == '_' ? Token{'d', -1*get_number()} : c >= '0' && c <= '9' ? Token{'d', get_number()} : Token{c};
}

int get_expression(void){

    int left = get_primary();
    Token t = ts.get_token();
    switch ( t.type )
    {
    case '+':
        return left+get_expression();
    case '-':
        return left-get_expression();
    case '*':
        return left*get_expression();
    case '/':
        return left/get_expression();
    default:
        ts.putback(t);
        return left;
    }
}

int get_primary(void){

    Token t = ts.get_token();
    switch ( t.type )
    {
    case 'd':
        return t.val;
    case '(':
        {
            int right = get_expression();
            t = ts.get_token();
            if ( t.type != ')' ) while ( true );
            return right;
        }
    case 'A' ... 'Z':
        {
            Token tmp = ts.get_token();
            if ( tmp.type == '=' )
            {
                int right = get_expression();
                if ( values[t.type] != right )
                    potential[t.type] = right;
                else
                    potential.erase(t.type);

                return right;
            }
            else
            if ( is_op(tmp.type) )
            {
                int right = get_expression();
                return tmp.type == '+' ? get_value(t.type)+right : tmp.type == '-' ? get_value(t.type)-right :
                        tmp.type == '*' ? get_value(t.type)*right : get_value(t.type)/right;
            }

            ts.putback(tmp);
            return get_value(t.type);

        }
    default:
        ts.putback(t);
        return 0;
    }
}

int get_number(void){

    char c; int ans = 0;

    while ( (ts.ss >> c) && c >= '0' && c <= '9' )
        ans = ans*10+c-'0';

    if ( ts.ss )
        ts.ss.putback(c);
    return ans;
}

int get_value(char c){

    map<char, int>::iterator it;
    if ( (it = potential.find(c)) != potential.end() )
        return it->second;

    return values[c];
}

bool is_op(char c){

    return c == '+' || c == '-' || c == '*' || c == '/';
}
