void int2str(int a, char* str)
{
	const char char_map[] = {'0','1','2','3','4','5','6','7','8','9'};
	if (a) {
		char buf[12];
		int s=0,i=0;
		if (a < 0) {
			str[0]='-';
			a=-a;
			s++;
		}
		while (a) {
			buf[i++]=char_map[a%10];
			a/=10;
		}
		while (i) {
			str[s++]=buf[--i];
		}
		str[s]=0;
	}
	else {
		str[0]='0';
		str[1]=0;
	}
}
