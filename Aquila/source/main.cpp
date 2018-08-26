//#include"core.hpp"



int main(){
    cout<<"hello world"<<endl;
    core A;
    A.w_wall(A.r_now(),v::left,v::wall);
    A.turn_l();
    //A.turn_l();
    A.go_st();
    A.w_wall(A.r_now(),v::left,v::wall);
    return 0;
}