#include <cstdio>
int main(){const char*s="#include <cstdio>%cint main(){const char*s=%c%s%c;%cprintf(s,10,34,s,34,10,10,10,10);%creturn 0;%c}%c";
printf(s,10,34,s,34,10,10,10,10);
return 0;
}
