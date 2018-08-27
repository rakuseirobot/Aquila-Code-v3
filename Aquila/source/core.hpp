//#include<iostream>
#define np nullptr
using namespace std;
using ll = long long int;
using ci = const int;
using bl = bool;

namespace v{
    //for all ,such as wall,move,and so on...
    ci left =0;
    ci front=1;
    ci back=3;
    ci right=2;
    //direction
    ci start = -1;
    ci unknown = 0;
    ci normal = 1;
    ci black=2;
    ci r_kit=3;
    ci c_point=4;
    ci slope=5;
    ci hisai=6;
    //type
    //ci unknown=0;//type�ｿｽ�ｿｽunknown�ｿｽﾆ一緒�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽA�ｿｽg�ｿｽ�ｿｽ�ｿｽﾜわす�ｿｽB
    ci wall=1;
    ci nowall=-1;
    //wall
}

struct node{
    int x=0,y=0,z=0,depth=1000,type=0;
    node* next[4] ={nullptr,nullptr,nullptr,nullptr};
    node* back[1] ={nullptr};
    int wall[4]={0};// 0|<=|2 wall[0].,if dir=1 then wall[v::left]=left.??????????????
    int flag=0;
};

class nodes{
    ci max=200;
    int size,now;
    node mal[200];
    bl box[200]={0};
public:
    nodes(){size=0;now=0;};
    node* make(){
        if(now>=max-1){ return np; }else{
            now++;
            return &mal[now];
        }
    }
    bl full(){ if(now>=max-1){ return true; }else{ return false; } }
};

nodes mall;

class core{
    node* now;
    node* start;//it may be unnecessary.But now, it is used in dfs. 
    int dir;//0::<,1::^,2::>,3::v
    //for status
    long long unsigned int counter;//dfs's counter.
    node* ans;//dfs's answer.
    //for dfs
public:
    core(){
        now = mall.make();
        start = now; 
        start->x=100;start->y=100;start->z=1;start->back[0]=np;start->type=v::start;start->depth=0;
        //dir = v::left;
		dir = 0;
        counter=0;
        ans = np;
    };
    void turn_r(){
        dir++;
        if(dir>3){ dir-=4; }
    };
    void turn_l(){
        dir--;
        if(dir<0){ dir+=4; }
    };

    void dfs(node* t,int x,int y,int z,int depth){
        if(t!=np){
            if(t->depth>depth+1)t->depth=depth+1;
            if((long long int)(t->flag)<=counter){
              if(t->x==x && t->y==y && t->z==z){ ans=t; }
              t->flag=counter+1;
              dfs(t->next[0],x,y,z,t->depth);
              dfs(t->next[1],x,y,z,t->depth);
              dfs(t->next[2],x,y,z,t->depth);
              dfs(t->next[3],x,y,z,t->depth);
            }
        }
    };
    node* find(int x,int y,int z){
        ans=np;
        dfs(start,x,y,z,start->depth);
        counter++;
        return ans;
    };

    void move_to(node* t,int x,int y,int z){//connect (t,node(x,y,z)).
        node* k = find(x,y,z);
        if(k==np){
            node* n = mall.make();
            n->x=x;n->y=y;n->z=z;
            for(int i=0;i<4;i++){ if(t->next[i]==np){t->next[i]=n; break;}}
            n->depth=t->depth +1;
            n->back[0]=t;
            n->next[0]=t;
        }else{
            if(t->next[0]!=k && t->next[1]!=k && t->next[2]!= k && t->next[3]!= k){
                for(int i=0;i<4;i++){ if(t->next[i]==np){t->next[i]=k; break;}}//conection of nodes.
                if(k->next[0]!=t && k->next[1]!=t && k->next[2]!= t &&k->next[3]!=t){ for(int i=0;i<4;i++){ if(k->next[i]==np){k->next[i]=t; break; } } }//conection of nodes.
                find(x,y,z);//depth�ｿｽﾌ更�ｿｽV
            }
            k->back[0]=t;
        }
    };
    void move_to(int x,int y,int z){
        move_to(now,x,y,z);
        now = find(x,y,z);
    }

	node* r_start(){ return start; }

    node* r_now(){ return now; }
    void w_now(int x,int y,int z){ now = find(x,y,z); }
    void w_now(node* ty){ now = ty; }

    int r_dir(){ return dir; }
    void w_wall(node* t,int direction,int num){ t->wall[(dir+direction+3)%4]=num; }
	void w_wall(int direction,int num){ now->wall[(dir+direction+3)%4]=num; }
 
    int r_wall(node* t,int direction){ return t->wall[(dir+direction+3)%4]; }
    int r_wall(int direction){ return now->wall[(dir+direction+3)%4]; }

    void go_st(){
        int x=now->x;
        int y=now->y;
        int z=now->z;
        switch(dir){
            case 0:
                x--;
                break;
            case 1:
                y++;
                break;
            case 2:
                x++;
                break;
            case 3:
                y--;
                break;
            default:
                break;
        }
        move_to(x,y,z);
    }

    int count_walls(node* t){
        int ans = 0;
        for(int i=0;i<4;i++)if(t->wall[i]==v::wall)ans++;
        return ans;
    }

    node* ac_next(node* t,int dire,int now_dir,int dist){//(node*,int,int,int)->node* | nullptr(error)
        int direction = (dire+now_dir+3)%4;
        switch(direction){
            case v::left:
                return find(t->x - dist,t->y,t->z);
                break;
            case v::front:
                return find(t->x,t->y + dist,t->z);
                break;
            case v::right:
                return find(t->x + dist,t->y,t->z);
                break;
            case v::back:
                return find(t->x,t->y - dist,t->z);
                break;
            default:
                return np;
                break;
        }
    }

	void append_node(node* t,int dire,int now_dir,int dist){
		if(ac_next(t,dire,now_dir,dist)==np){
			int direction = (dire+now_dir+3)%4; 
			switch(direction){
				case v::left:
				    move_to(t,t->x - dist,t->y,t->z);
				    break;
				case v::front:
				    move_to(t,t->x,t->y + dist,t->z);
				    break;
				case v::right:
				    move_to(t,t->x + dist,t->y,t->z);
				    break;
				case v::back:
				    move_to(t,t->x,t->y - dist,t->z);
			    	break;
				default:
				    //np;
				    break;
			}
		}else{
			//no action
		}
	}
	void append_node(int dire,int dis){
		append_node(now,dire,dir,dis);
	}

    node* ac_next(int dire,int dist){ return ac_next(now,dire,dir,dist); }
};

