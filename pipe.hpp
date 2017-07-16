namespace pipe_{

// ok?
const int READ = 0, WRITE = 1;
struct Pipe{
    int p[2];
    bool closed[2] = {};
    Pipe(){
        if(pipe(p) < 0){
            throw "fail to open pipe";
        }
    }
    ~Pipe(){
        if(!closed[READ]){ close(p[READ]); }
        if(!closed[WRITE]){ close(p[WRITE]); }
    }
    void close(bool rw){
        ::close(p[rw]);
        closed[rw] = true;
    }
    void dup2(bool rw, int fd){
        if(::dup2(p[rw], fd) < 0){ throw "fail to dup2"; }
        close(rw);
    }
};
void pp2(string exe){
    Pipe cp, pc;
    int pid = fork();
    if(pid < 0){ throw "fail to fork"; }
    // prints<cerr>("pid", pid);
    if(pid == 0){
        pc.close(WRITE);
        cp.close(READ);
        pc.dup2(READ, STDIN_FILENO);
        cp.dup2(WRITE, STDOUT_FILENO);
        prints<cerr>("exec", exe.c_str());
        if(execl(exe.c_str(), exe.c_str(), 0) < 0){ throw string("fail to exec ") + exe; }
        // throw "something goes wrong after exec";
    }else{
        pc.close(READ);
        cp.close(WRITE);
        pc.dup2(WRITE, STDOUT_FILENO);
        cp.dup2(READ, STDIN_FILENO);
    }
}

}
