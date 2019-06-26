#include <stdio.h>

struct io{
	int exist, start_time  , burst_time , io_complete;
};

typedef struct _process{
	int pid, arrival_time, burst_time, priority, remain_time, ready_time, complete;
	struct io io;
	
}process;


process info[20];

// 전체 프로세스 개수. 
int i,number;



void enter(){

	//프로세스 정보 입력받기
	printf("enter number of process:\t");
	scanf("%d", &number);
	printf("\nenter information about processes\n");
	printf("(※assume higher priority number implies higher priority and there is no process that io start time is 0.)\n"); 
	
	
	for(i=0; i<number; i++){
		printf("\n==process[%d]==\n", i+1);
		info[i].pid = i+1;
		info[i].io.io_complete = 0;
		info[i].complete = 0;
	
		printf("Arrival Time?:\t");
		scanf("%d", &info[i].arrival_time);
		info[i].ready_time = info[i].arrival_time;
		printf("Burst Time?:\t");
		scanf("%d", &info[i].burst_time);
		info[i].remain_time = info[i].burst_time;
		printf("Priority?:\t");
		scanf("%d", &info[i].priority); 
		
		printf("does this process operates i/o  operation? if yes enter 1 else enter 0:\t");
		scanf("%d", &info[i].io.exist);
		if (info[i].io.exist == 1){ 
			printf("enter i/o start time:\t");
			
			scanf("%d", &info[i].io.start_time);
			printf("enter i/o burst time:\t");
			scanf("%d", &info[i].io.burst_time);
		}
		else{
			info[i].io.start_time = -1;
		}
		
	} 
	
	
}


void evaluate(int waiting_time[], int turnaround_time[] ){
	printf("\npid     waiting_time      turnaround_time\n");
	printf("======================================================\n");
	int waiting_sum =0,turnaround_sum = 0;
	for (i=1; i<=number; i++){
		printf("%d\t\t%d\t\t%d\t\t\n", i, waiting_time[i], turnaround_time[i]);
		waiting_sum+=waiting_time[i];
		turnaround_sum+=turnaround_time[i];
	}
	
	printf("\n======================================================\n");
	float n = number;
	

	printf("average_waiting_time:\t%lf\t\t average_turnaround_time:\t%lf\t\t", waiting_sum/n, turnaround_sum/n);

}

void ready_sort(){
	// 선택정렬으로 ready_time 빠른순으로 info 배열(프로세스 정보들 저장되어 있음) 정렬함. 
	for (i=0; i<number-1; i++){
		int min = i,j;
		process temp ;
		for (j = i+1; j<number; j++ ){
			if (info[min].ready_time > info[j].ready_time ){
				min = j;
			}
		}
		temp = info[i];
		info[i] = info[min];
		info[min] = temp;
		
    }
	
	
}



void FCFS(){
	int waiting_time[number+1];
	int turnaround_time[number+1];
	// time => 현재 시간 / x -> 종료되지 않은 프로세스 개수 / id -> 탐색할 프로세스 인덱스. 
	int time = 0, x = number, id=0;
	
	ready_sort();

	 
	///////////////////////////////////// 
	printf("\n====================GHANT CHART=======================\n");
	
	while(x>0){
		if (info[id].complete == 0){
			// 해당 프로세스가 종료되지 않은 경우 다음 코드 진행. 
			
			if(info[id].ready_time > time){
				// 정렬했으므로 id 가 가장 ready_time 빠른프로세스인데 아직 시간이 안됐다는 것은 현재 수행할 프로세스가 없다는 것. 
				printf("x");
				time++;
			}
			else{
				if(info[id].io.exist != 1){
					// i/o 작업을 하지 않는 프로세스 
					// 이경우에는 프로세스들의 remain_time에 변화가 없으므로 다시 정렬할 필요 없음. 
					for(i=0; i<info[id].burst_time; i++){
						printf("%d", info[id].pid);
					}
					time += info[id].burst_time;
					x--;
					info[id].complete = 1;
					
					
					
					turnaround_time[info[id].pid] = time - info[id].arrival_time ;
					waiting_time[info[id].pid] = turnaround_time[info[id].pid] - info[id].burst_time;
					
					id = (id+1)%number;
				}
				else if(info[id].io.io_complete == 0){
					// 아직 io 작업 수행하지 않은 프로세스.
					for(i=0; i<info[id].io.start_time; i++) {
						printf("%d", info[id].pid);
					}
					time += info[id].io.start_time;
					info[id].io.io_complete = 1;
					info[id].ready_time = time + info[id].io.burst_time;
					info[id].remain_time = info[id].burst_time - info[id].io.start_time;
					// ready_time에 변화가 생겼으므로 다시 정렬해줌.
					ready_sort();
					id = number-x; // 안끝난 프로세스중 가장 빠른 ready_time 
				} 
				else{
					// i/o 작업 수행하고 온 프로세스. 
					for(i=0; i<info[id].remain_time; i++){
						printf("%d", info[id].pid);
					}
					time += info[id].remain_time;
					x--;
					info[id].complete = 1;
					
					
					turnaround_time[info[id].pid] = time - info[id].arrival_time ;
					waiting_time[info[id].pid] = turnaround_time[info[id].pid] - info[id].burst_time;
					
					id = (id+1)%number;
				}
			}
			
		}
		else{
			// 해당 프로세스가 종료되었다면 다음 인덱스 값으로.
			 
			id = (id+1)%number;
		}
	}
	

	

	printf("\n======================================================\n");
	
    evaluate(waiting_time, turnaround_time);
}


void SJF_sort(){
	    // burst 가 작은순으로 info 정렬. 
		for (i=0; i<number-1; i++){
		int min = i,j;
		process temp ;
		for (j = i+1; j<number; j++ ){
			if (info[min].remain_time > info[j].remain_time ){
				min = j;
			}
		}
		temp = info[i];
		info[i] = info[min];
		info[min] = temp;
		
    }
    
	
}

void non_preemptive_SJF(){
	//printf("non_preemptive_SJF!\n");
	int waiting_time[number+1];
	int turnaround_time[number+1];
	
    SJF_sort();
    
    
    /////////////////
    printf("\n====================GHANT CHART=======================\n");
    int time = 0;
    // x -> 완료되지 않은 프로새스 개수. 
    int x = number;
    int id = 0;
    
    while(x>0){
    	if (info[id].complete==0){
    		
    		
    		if(info[id].ready_time > time){
    	    	
    	    	if(id==number-1){
    				id = 0;
    				time++;
    				printf("x");
				}
				else{
					id++;
				}
				
    	    	
			}
			
				
			else if(info[id].io.exist != 1){
				
				x--;
				info[id].complete = 1;
				time += info[id].burst_time;
			
				for(i=0; i<info[id].burst_time; i++){
					printf("%d", info[id].pid);
				}
				
				turnaround_time[info[id].pid] = time - info[id].arrival_time;
				waiting_time[info[id].pid] = turnaround_time[info[id].pid] - info[id].burst_time;
					
				//printf("qqqqqq");
				SJF_sort(); 
				id = 0;
			}
		
		
			else{
				if(info[id].io.io_complete==0){
					//printf("??%d$%d??", id, info[id].pid);
					
					time+=info[id].io.start_time;
					info[id].ready_time = time+info[id].io.burst_time;
					for(i=0; i<info[id].io.start_time; i++){
						printf("%d", info[id].pid);
						
					}
					info[id].io.io_complete = 1;
					info[id].remain_time = info[id].burst_time - info[id].io.start_time;
					SJF_sort();
					id = 0;
				}
				
				else{
					
					time+=info[id].remain_time;
					x--;
					info[id].complete = 1;
					for(i=0; i<info[id].remain_time; i++){
						printf("%d", info[id].pid);
					}
					
					turnaround_time[info[id].pid] = time - info[id].arrival_time;
					
					waiting_time[info[id].pid] = turnaround_time[info[id].pid] - info[id].burst_time;
					
					
					SJF_sort();
					id = 0;
				}
				
			
			}	

		}
		else{
			id = (id+1)%number;
		}
    	

    	
	}
	printf("\n======================================================\n");
	//////////////////////////////////////////////
	
	
	evaluate(waiting_time, turnaround_time);
	
}




void preemptive_SJF(){
	
	//printf("preemptive_SJF\n");
	int waiting_time[number+1];
	int turnaround_time[number+1];
	
    SJF_sort();
    

    /////////////////

    printf("\n====================GHANT CHART=======================\n");
	
    int time;
    // x -> 완료되지 않은 프로새스 개수. 
    int x = number;
    int id = 0;

	for(time =0; x>0; time++){
		
		
		if(info[id].complete == 0){
			
			if(info[id].ready_time > time){
				//printf("time?%d\tready?%d\n", time,info[id].ready_time);
				

    	    	if(id==number-1){
    				id = 0;
    		
    				printf("x");
				}
				else{
					id++;
					time--;
				}

//				int count = 0;
//				while(info[id].ready_time > time && count < x ){
//					id = (id+1)%number;
//					count++;
//					
//				}
//				if(count == x){
//					
//					printf("x");
//					
//				}
					
					
			}
			
			
			
			else{
				
				//printf("time?%d\tready?%d\n", time,info[id].ready_time);
				if(info[id].remain_time > 1){
					if(info[id].io.start_time==1){
						// 이번 작업 끝나면 io interrupt 발생할 프로세스
						info[id].ready_time = time+1+info[id].io.burst_time;
						info[id].io.start_time --;
						
						printf("%d", info[id].pid) ;
						info[id].remain_time --;
						SJF_sort();
						id = 0;
						
					}
					else{
						//printf("qqqqqqqqqq");
						info[id].io.start_time --;
						printf("%d", info[id].pid);
						info[id].remain_time --;
						SJF_sort();
						id = 0;
						
					}
					
				
					
					
				}
				
				
				else{
					// 마지막수행! 이거하면 종료되는 프로세스
					printf("%d", info[id].pid);
					x--;
					info[id].remain_time --;
					SJF_sort();
					info[id].complete = 1;
					turnaround_time[info[id].pid] = time+1 - info[id].arrival_time;
					waiting_time[info[id].pid] = turnaround_time[info[id].pid] - info[id].burst_time;
					
					id = 0;
				
					
					//printf("\n complete? =>%d////%d",info[id].complete, info[id].remain_time);
					
				} 
				
			}
		}
		else{
			// id 에 해당하는 프로세스가 이미 끝난 작업일 경우 그냥 패스.
			//printf("no...%d", info[id].pid);
		 
			id = (id+1)%number;
			time--;
		}
	}


	printf("\n======================================================\n");
	//////////////////////////////////////////////
	

	
	evaluate(waiting_time, turnaround_time);
	
	
	
	
	
}




void priority_sort(){
	
	
		for (i=0; i<number-1; i++){
		int max = i,j;
		process temp ;
		for (j = i+1; j<number; j++ ){
			if (info[max].priority < info[j].priority ){
				max = j;
			}
		}
		temp = info[i];
		info[i] = info[max];
		info[max] = temp;
		
    }
    
	
}



void non_preemptive_priority(){
	//printf("non_preemptive_priority\n");
	int waiting_time[number+1];
	int turnaround_time[number+1];
	
    priority_sort();

    
    /////////////////
    
    printf("\n====================GHANT CHART=======================\n");
    int time = 0;
    // x -> 완료되지 않은 프로새스 개수. 
    int x = number;
    int id = 0;
    while(x>0){
    	if (info[id].complete==0){
    		
    		
    		if(info[id].ready_time > time){
    	    	
    	    	
    	    	int count = 0;
				while(info[id].ready_time > time && count < x ){
					id = (id+1)%number;
					count++;
				}
				if(count == x){
					time++;
					printf("x");
					
				}
				
    	    	
    	    	
			}
			
				
			else if(info[id].io.exist != 1){
				
				x--;
				info[id].complete = 1;
				time += info[id].burst_time;
			
				for(i=0; i<info[id].burst_time; i++){
					printf("%d", info[id].pid);
				}
				
				turnaround_time[info[id].pid] = time - info[id].arrival_time;
				waiting_time[info[id].pid] = turnaround_time[info[id].pid] - info[id].burst_time;
			
				id = 0;
			}
		
		
			else{
				if(info[id].io.io_complete==0){
					//printf("??%d$%d??", id, info[id].pid);
					
					time+=info[id].io.start_time;
					info[id].ready_time = time+info[id].io.burst_time;
					for(i=0; i<info[id].io.start_time; i++){
						printf("%d", info[id].pid);
						info[id].io.io_complete = 1;
						info[id].remain_time = info[id].burst_time - info[id].io.start_time;
				 }
				
					id = 0;
				}
				
				else{
					
					time+=info[id].remain_time;
					x--;
					info[id].complete = 1;
					for(i=0; i<info[id].remain_time; i++){
						printf("%d", info[id].pid);
					}
					
					turnaround_time[info[id].pid] = time - info[id].arrival_time;
					
					waiting_time[info[id].pid] = turnaround_time[info[id].pid] - info[id].burst_time;
					
				
					id = 0;
				}
				
			
			}	

		}
		else{
			id = (id+1)%number;
		}
    	

    	
	}
	printf("\n======================================================\n");
	//////////////////////////////////////////////

	evaluate(waiting_time, turnaround_time);


	
}

void preemptive_priority(){
	// preemptive SJF 랑 다른점은 priority는 안변하도록 설계했으므로 정렬 초기에 한번만 하면됨..
	// SJF 때에는 프로세스 한번이라도 수행한경우 SJF_sort()진행하고 id=0으로 바꿔주었었음. 
	
	//printf("preemptive_priority\n");
	int waiting_time[number+1];
	int turnaround_time[number+1];
	
    priority_sort();
    

    /////////////////

    printf("\n====================GHANT CHART=======================\n");
	
    int time;
    // x -> 완료되지 않은 프로새스 개수. 
    int x = number;
    int id = 0;

	for(time =0; x>0; time++){
		//printf("\ntime => %d\tid=>%d\t", time,id);
		
		if(info[id].complete == 0){
			// 아직 완료 되지 않은 프로세스만 탐색. 
			if(info[id].ready_time > time){
				// cpu 수행할 준비가 되었는가 확인.....  
				

//    	    	if(id==number-1){
//    				id = 0;
//    		
//    				printf("x");
//				}
//				else{
//					id++;
//					time--;
//				}
				int count = 0;
				while(info[id].ready_time > time && info[id].complete==0 && count < x ){
					id = (id+1)%number;
					count++;
					
				}
				
				if(count == x){
					
					printf("x");
					
				
				}
				else{
					time--;
				}
				




					
				
			}
			
			
			
			else{
				// 레디 상태인 프로세스! 
				
	
				if(info[id].remain_time > 1){
					// 이번 수행이 마지막이 아닌 경우... 
					if(info[id].io.start_time==1){
						// 이번 작업 끝나면 io interrupt 발생할 프로세스
						info[id].ready_time = time+1+info[id].io.burst_time;
						
						info[id].io.start_time --;
					
						printf("%d", info[id].pid) ;
						info[id].remain_time --;
						id = 0;
						
					}
					else{
						
						info[id].io.start_time --;
					
						printf("%d", info[id].pid);
						info[id].remain_time --;
						id = 0;
						
					}
					
				
					
					
				}
				
				
				else{
					// 마지막수행! 이거하면 종료되는 프로세스
				
					printf("%d", info[id].pid);
					x--;
					info[id].remain_time --;
				
					info[id].complete = 1;
					turnaround_time[info[id].pid] = time+1 - info[id].arrival_time;
					waiting_time[info[id].pid] = turnaround_time[info[id].pid] - info[id].burst_time;
					
					id = 0;
				
					
					//printf("\n complete? =>%d////%d",info[id].complete, info[id].remain_time);
					
				} 
				
			}
		}
		else{
			// id 에 해당하는 프로세스가 이미 끝난 작업일 경우 그냥 패스.
			//printf("no...%d", info[id].pid);
		 
			id = (id+1)%number;
			time--;
		}
	}


	printf("\n======================================================\n");
	//////////////////////////////////////////////
	

	
	evaluate(waiting_time, turnaround_time);
	
	
	
	
}

void RR(){
	//printf("RR!\n");
	int waiting_time[number+1];
	int turnaround_time[number+1];
	
	int time_quantum, time = 0, x = number, id=0;
	
	printf("enter time quantum.:\t");
	scanf("%d", &time_quantum);


  	/////////////////

    printf("\n====================GHANT CHART=======================\n");

 	while(x>0){
 		//printf("x??=> %d\n", x);
 		
 		//printf("\ntime=>%d\n", time);
    	if (info[id].complete==0){
    		
    		
    		if(info[id].ready_time > time){
//    	    	if(id == number-1){
//    				id = 0;
//    				time++;
//    				printf("x");
//    				
//				}
//				else{
//					//printf("\nid++");
//					
//					id++;
//				}



				int count = 0;
				while(info[id].ready_time > time && count < x ){
					id = (id+1)%number;
					count++;
				}
				if(count == x){
					time++;
					printf("x");
				}
				
				
				


			}
			
			
			else{
				if(info[id].remain_time > time_quantum && (info[id].io.start_time > time_quantum || info[id].io.start_time <= 0)){
				// 일반적으로 타임퀀텀 만큼 수행하고 턴 넘겨주기.
					//printf("\n1");
					info[id].remain_time -= time_quantum;
					info[id].io.start_time -= time_quantum;
					for(i=0; i<time_quantum; i++){
						printf("%d", info[id].pid);
					} 
					time+=time_quantum;
					id = (id+1)%number;
					
				}
				// io 인터럽트 발생까지 남은 시간이나 remain_time 이 타임퀀텀 보다 작을경우 항상 인터럽트까지 남은 시간이 더 적을 것이므로...(프로세스 burst time 안에 io 인터럽트 발생하도록 지정해줬으니까) 
				else if(0<info[id].io.start_time && info[id].io.start_time <= time_quantum){
					//printf("\n2");
					
					time+=info[id].io.start_time;
					info[id].ready_time = time+info[id].io.burst_time;
					info[id].remain_time -= info[id].io.start_time;
					for(i=0; i<info[id].io.start_time; i++){
						printf("%d", info[id].pid);
					}
					info[id].io.start_time = -1;
					id = (id+1)%number;
					
					
				}
				else{
				// 해당 프로세스의 마지막 수행..
					//printf("\n3");
					x--;
					info[id].complete = 1;
					time += info[id].remain_time;
					
					turnaround_time[info[id].pid] = time - info[id].arrival_time;
					waiting_time[info[id].pid] = turnaround_time[info[id].pid] - info[id].burst_time;
					
					for(i=0; i<info[id].remain_time; i++){
						printf("%d", info[id].pid);
					}	
					id = (id+1)%number;
					
				}
			}
				
			

		}
		else{
			//printf("\ndd");
			id = (id+1)%number;
		}
    	

    	
	}
	printf("\n======================================================\n");
	//////////////////////////////////////////////
	
	
	
	
	evaluate(waiting_time, turnaround_time);	
	
}


int main(){
	
	int mode;
	
	enter();
	printf("\npick scheduler.\n");
	printf("1.FCFS\n2.non preemptive SJF\n3.preemptive SJF\n4.non preemptive priority\n5.preemptive priority\n6.Round Robin\n") ;
    printf("enter number:\t");
    scanf("%d", &mode);
    switch(mode){
    	case 1:
    		FCFS();
			break;
			
		case 2:
			non_preemptive_SJF();
			break;
		case 3:
		    preemptive_SJF();
			break;
		case 4:
		    non_preemptive_priority();
			break;
		case 5:
		    preemptive_priority();
		    break;
		case 6:
		    RR();
			break;		
		default:
		    printf("Wrong Number.\n");
			break;			
				
	}
	
}
