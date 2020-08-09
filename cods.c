#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#define PI 3.14159265
typedef struct Point{
	char name[5];
	float x;
	float y;
}point;
typedef struct Line{
	char name[5];
	point p1;
	point p2; 

}line;
typedef struct Polygon{
	char name[5];
	point points[40];	

}polygon;
float distance(float x1,float y1,float x2,float y2){
	float gdistance;
	gdistance = ((x2-x1)*(x2-x1))+((y2-y1)*(y2-y1));
	gdistance = sqrt(gdistance);
	return gdistance;

}
float angle(line l1,line l2){
	float result;
	result=(acos(((l1.p1.x-l1.p2.x)*(l2.p1.x-l2.p2.x)+(l1.p1.y-l1.p2.y)*(l2.p1.y-l2.p2.y))/(distance(l1.p1.x,l1.p1.y,l1.p2.x,l1.p2.y)*distance(l2.p1.x,l2.p1.y,l2.p2.x,l2.p2.y))))*(180/PI);
    return result;
}
float lenght(polygon p){
	int i=0,counter=0,j;
	float h=0.0;
	for(int j=0;j<100;j++){
		if(p.points[j].name[0]!='P'){
			break;
		}
		counter++;
	}
	while(i!=counter){
		h += distance(p.points[i].x,p.points[i].y,p.points[i+1].x,p.points[i+1].y);
		i++;
	}
	return h;
}
float area(polygon p){
	int i=0,counter=0,j;
	float h=0.0;
	for(int j=0;j<100;j++){
		if(p.points[j].name[0]!='P'){
			break;
		}
		counter++;
	}
	while(i!=counter){
		h+= (p.points[i].x*p.points[i+1].y)-(p.points[i+1].x*p.points[i].y);
		i++;
	}
	h = abs(h);
	return h/2;
}
void reading(char array[25][100],char* file_path,int *hold){
	FILE *fp;
	int i=0;
	fp = fopen(file_path,"r");
	while(!feof(fp)){
		fgets(array[i],100,fp);
		i++;
	}
	*hold = atoi(array[1]);
	fclose(fp);
}
void fill_point(char array[25][100],point *points,int *hold){
	int i=0,j=1;
	float f1,f2;
	char name[2];
	while(i+j-1<*hold){
		if(*hold == atoi(array[i])){
			while(array[i+j][0] != 'P'){
				sscanf(array[i+j],"%f %f %s",&f1,&f2,name);
				points[j-1].x = f1;
				points[j-1].y = f2;
				strcpy(points[j-1].name,name);
				j++;
			}
		}
		i++;
	}
	*hold=*hold-(j-1);
}
void fill_line(char array[25][100],line *lines,point *points,int *hold){
	int i=0,a,b=0;
	char p1[5],p2[5],name[5];
	while(array[i][0] != 'a'){
		if(array[i][6] == 'L'){
			sscanf(array[i],"%s %s %s",p1,p2,name);
			for(a=0;a<*hold;a++){
				if(strcmp(p1,points[a].name) == 0){
					lines[b].p1 = points[a];
				}
				if(strcmp(p2,points[a].name) == 0){
					lines[b].p2 = points[a];
				}
			}
			strcpy(lines[b].name,name);
			b++;
		}
		i++;
	}
	*hold=*hold-b;
}
void polygon_lines(char array[25][100],char temp[100][100]){
	int i=0,j,a=0;
	while(i!=25){
		if(array[i][6] == 'P' || array[i][8] == 'L'){
			for(j=0;j<strlen(array[i]);j++){
				if((array[i][j] == ' ' && array[i][j+1] == ' ') || array[i][j] == '/'){
					break;
				}
				else{
					temp[a][j] = array[i][j];
				}
			}
			temp[a][j] = '\0';
			a++;	
		}
		i++;
	}
}
void fill_something(char polygon[100][100],char name[100][5],char p[100][40][5],int *hold){
	int i=0,j=0,counter=0,counter2=0,a=0;
	while(a!=*hold){
		i=0;
		while(polygon[a][i] != '\n'){
			if(polygon[a][i] == 'P'){
				if(polygon[a][i+1] == 'G'){
					for(j=0;j<4;j++){
						name[counter][j] = polygon[a][i+j];
					}
					name[counter][j] = '\0';
					counter++;
				}
			}
			i++;
		}
		a++;
	}
	i=0;
	counter=0;
	a=0;
	while(a!=*hold){
		i=0;
		while(polygon[a][i] != '\n'){
			if(polygon[a][i] == 'P' && polygon[a][i+1] == 'G'){
				break;
			}
			if(polygon[a][i] == 'P'){
				if(polygon[a][i+1] != 'G'){
					for(j=0;j<3;j++){
						p[a][counter][j] = polygon[a][i+j];
					}
					p[a][counter][j-1] = '\0';
					counter++;
				}
			}
			else if(polygon[a][i] == 'L'){
				for(j=0;j<3;j++){
					p[a][counter2][j] = polygon[a][i+j];
				}
				p[a][counter2][j] = '\0';
				counter2++;
			}
			i++;
		}
		a++;
	}

}
void fill_polygon(char array[25][100],polygon *polygons,line *lines,point *points,int *hold){
	int a = *hold,i=0,j;
	char polygon[*hold][100];
	char name[*hold][5];
	char p[*hold][40][5];
	polygon_lines(array,polygon);
	fill_something(polygon,name,p,&a);
	for(i=0;i<*hold;i++){
		strcpy(polygons[i].name,name[i]);
	}
	i=0;
	while(i!=*hold){
		for(j=0;j<40;j++){
			if(strcmp(p[i][j],points[j].name) == 0){
				polygons[i].points[j] = points[j];
			}
			else if(strcmp(p[i][j],lines[j].name) == 0){
				polygons[i].points[j] = lines[j].p1;
				polygons[i].points[j+1] = lines[j+1].p2;
			}
		}
		i++;
	}
}
void two_points_distance(char array[25][100],point *points,char output[100]){
	FILE * fp;
	fp = fopen (output, "a");
	int i=0,j,a=0,counter=0,counter2=0;
	float x;
	char p1[5],p2[5],temp[20];
	point a1[100];
	point a2[100];
	while(i!=25){
		if(array[i][0] == 'D'){
			sscanf(array[i],"%s %s %s",temp,p1,p2);
			if(p1[0] == 'P' && p2[0] == 'P'){
				for(j=0;j<100;j++){
					if(strcmp(p1,points[j].name) == 0){
						a1[a] = points[j];			
					}
					else if(strcmp(p2,points[j].name) == 0){
						a2[a] = points[j];
					}
				}
				a++;
			}
		}
		i++;
	}
	for(i=0;i<a;i++){
		x = distance(a1[i].x,a1[i].y,a2[i].x,a2[i].y);
		fprintf(fp,"Distance(%s,%s) = %.1f\n",a1[i].name,a2[i].name,x);
	}
	
	fclose(fp);
}
float calculate(point p,line l){
	float h,m1;
	m1 = ((l.p2.y)-(l.p1.y))/((l.p2.x)-(l.p1.x));
	if(l.p1.x == l.p2.x){
		h = p.x-l.p1.x;	
	}
	else{	
		h = fabs((-m1*p.x)+(1*p.y)+(m1*l.p2.x-l.p2.y))/sqrt((m1*m1+1));
	}
	return h;

}
void points_between_lines(char array[25][100],point *points,line *lines,char output[100]){
	FILE * fp;
	fp = fopen (output, "a");
	int i=0,j,a=0,counter=0,counter2=0;
	float x;
	char p1[5],p2[5],temp[20];
	point a1[100];
	line a2[100];
	while(i!=25){
		if(array[i][0] == 'D'){
			sscanf(array[i],"%s %s %s",temp,p1,p2);
			if(p1[0] == 'P' && p2[0] == 'L'){
				for(j=0;j<100;j++){
					if(strcmp(p1,points[j].name) == 0){
						a1[a] = points[j];					
					}
					if(strcmp(p2,lines[j].name) == 0){
						a2[a] = lines[j];
					}
				}
				a++;
			}
		}
		i++;
	}
	for(i=0;i<a;i++){
		x = calculate(a1[i],a2[i]);
		fprintf(fp,"Distance(%s,%s) = %.1f\n",a1[i].name,a2[i].name,x);
	}
	
	fclose(fp);

}
void find_angel(char array[25][100],line *lines,char output[100]){
	FILE * fp;
	fp = fopen (output, "a");
	int i=0,j,a=0;
	float x;
	char p1[5],p2[5],temp[20];
	line a1[100];
	line a2[100];
	while(i!=25){
		if(array[i][0] == 'A' && array[i][1] == 'n' ){
			sscanf(array[i],"%s %s %s",temp,p1,p2);
			for(j=0;j<100;j++){
				if(strcmp(p1,lines[j].name) == 0){
					a1[a] = lines[j];					
				}
				if(strcmp(p2,lines[j].name) == 0){
					a2[a] = lines[j];
				}
			}
			a++;
		}
		i++;
	}
	for(i=0;i<a;i++){
		x = angle(a1[i],a2[i]);
		fprintf(fp,"Angle(%s,%s) = %.1f\n",a1[i].name,a2[i].name,x);
	}
	fclose(fp);

}
void find_lenght(char array[25][100],polygon *polygons,char output[100]){
	FILE * fp;
	fp = fopen (output, "a");
	int i=0,j,a=0;
	float x;
	char p1[5],temp[20];
	polygon a1[100];
	while(i!=25){
		if(array[i][1] == 'e'){
			sscanf(array[i],"%s %s",temp,p1);
			for(j=0;j<100;j++){
				if(strcmp(p1,polygons[j].name) == 0){
					a1[a] = polygons[j];
				}					
			}
			a++;
		}
		i++;
	}
	for(i=0;i<a;i++){
		x = lenght(a1[i]);
		fprintf(fp,"Lenght(%s) = %.1f\n",a1[i].name,x);
	}
	fclose(fp);
}
void find_area(char array[25][100],polygon *polygons,char output[100]){
	FILE * fp;
	fp = fopen (output, "a");
	int i=0,j,a=0;
	char p1[5],temp[20];
	float x;
	polygon a1[100];
	while(i!=25){
		if(array[i][1] == 'r'){
			sscanf(array[i],"%s %s",temp,p1);
			for(j=0;j<100;j++){
				if(strcmp(p1,polygons[j].name) == 0){
					a1[a] = polygons[j];
				}					
			}
			a++;
		}
		i++;
	}
	for(i=0;i<a;i++){
		x = area(a1[i]);
		fprintf(fp,"Area(%s) = %.1f\n",a1[i].name,x);
	}
	fclose(fp);
}
void read_output(char array[25][100],char output[100]){
	int i=0;
	char temp[100];
	while(i!=25){
		sscanf(array[i],"%s",temp);
		if(strcmp(temp,"actions") == 0){
			strcpy(output,array[i+1]);
		}
		i++;
	}
}
int main(){
	int i,hold;
	char output[100];
	char array[25][100];
	point points[10];
	line lines[100];
	polygon polygons[100];
	reading(array,"commands.txt",&hold);
	read_output(array,output);
	FILE *fp;
	fp = fopen(output,"w");
	fill_point(array,points,&hold);
	fill_line(array,lines,points,&hold);
	fill_polygon(array,polygons,lines,points,&hold);
	two_points_distance(array,points,output);
	points_between_lines(array,points,lines,output);
	find_angel(array,lines,output);
	find_lenght(array,polygons,output);
	find_area(array,polygons,output);
	return 0;
}
