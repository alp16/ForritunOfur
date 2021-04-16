#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#define SIZE 16
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
/*
  TODO: 
  Setja fiskana í random stærðir.
  Setja random Capacity á bátana.
  googla glans.
  Arnar muna eftir glösum.
  Minna Þorstein á að taka bjórinn og teemo hattinn.
  Passa að bátarnir klessi ekki á hvorn annan.
  Öldur og Stormur.
*/

int main (int argc, char** argv) {
  int numtasks, rank, source, dest, outbuf, i, tag=1;
  int inbuf[4]={MPI_PROC_NULL,MPI_PROC_NULL, MPI_PROC_NULL,  
        MPI_PROC_NULL};
  int nbrs[4];
  int dims[2] = {4,4}, periods[2] = {1,1}, reorder=0;
  int coords[2];
  int boat1[2], boat2[2];
  int fish1[2], fish2[2];
  int fish1Rank, fish2Rank, boat1Rank, boat2Rank;
  MPI_Comm cartcomm;
  
  MPI_Request reqs[8];
  MPI_Status stats[8];
  
 
  MPI_Init(&argc, &argv);
  
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
  MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods,reorder, &cartcomm);

  MPI_Comm_rank(cartcomm, &rank);
  
  MPI_Cart_coords(cartcomm, rank, 2, coords);
  
  MPI_Cart_shift(cartcomm, 0, 1, &nbrs[UP], &nbrs[DOWN] );
  MPI_Cart_shift(cartcomm, 1, 1, &nbrs[LEFT], &nbrs[RIGHT] );

  if(rank==0){
    printf("rank= %d coords= %d %d having neighbours(u,d,l,r)=%d %d %d %d \n", rank, 
          coords[0], coords[1], nbrs[UP], nbrs[DOWN], nbrs[LEFT], nbrs[RIGHT]);

    
    time_t t;
    srand((unsigned) time(&t));
    
    int randfishpos1 = rand() % 15 + 1;
    int randfishpos2 = rand() % 15 + 1;
    int randboatpos1 = rand() % 15 + 1;
    int randboatpos2 = rand() % 15 + 1;
    if(randboatpos1==randboatpos2){
      if(randboatpos2 !=1){
        randboatpos2 = randboatpos2 -1;
      }else{
        randboatpos2 = randboatpos2 +1;
      }
    }
    MPI_Cart_coords(cartcomm, randfishpos1, 2, fish1);
    MPI_Cart_coords(cartcomm,randboatpos1,2,boat1);
    MPI_Cart_coords(cartcomm, randfishpos2, 2, fish2);
    MPI_Cart_coords(cartcomm,randboatpos2,2,boat2);
    printf("Fish 1 starts at pos: %d,%d \n",fish1[0],fish1[1]);
    printf("Fish 2 starts at pos: %d,%d \n",fish2[0],fish2[1]);
    printf("Boat 1 starts at pos: %d,%d \n",boat1[0],boat1[1]);
    printf("Boat 2 starts at pos: %d,%d \n",boat2[0],boat2[1]);

    int tilbaka = 0;
    int counter = 50;
    int boat1Cap = 5;
    int boat2Cap = 5;
    int fish1Cap = 10;
    int fish2Cap = 10;
    int cap = 5;
    int totalFish = 0;
    int boat1GO = 0;
    int boat2GO = 0;
    int boat1Target[2];
    int boat2Target[2];
    MPI_Cart_coords(cartcomm,randboatpos1,2,boat2Target);
    MPI_Cart_coords(cartcomm,randboatpos2,2,boat2Target);
    while(tilbaka==0){

      int randFishDir1 = rand() % 4;
      if(fish1Cap>0){
        if(randFishDir1 == UP){
          if(fish1[0]!=1 || fish1[1] !=0){
            fish1[0] = fish1[0] - 1;
            if(fish1[0]<0){
              fish1[0] = 3; 
            }
          }
        }
        
        else if (randFishDir1== DOWN){
          if(fish1[0]!=3 || fish1[1] !=0){
            fish1[0] = fish1[0] + 1;
            if(fish1[0]>3){
              fish1[0] = 0;
            }
          }
        }
        
        else if (randFishDir1== RIGHT){
          if(fish1[0]!=0 || fish1[1] !=3){
            fish1[1] = fish1[1] + 1;
            if(fish1[1]>3){
              fish1[1]=0;
            }
          }
        }
        
        else{
          if(fish1[0]!=0 || fish1[1] !=1){
            fish1[1] = fish1[1] - 1;
            if(fish1[1]<0){
              fish1[1]=3;
            }
          }
        }
      }

     int randFishDir2 = rand() % 4;
      if(fish2Cap>0){
        if(randFishDir2 == UP){
          if(fish2[0]!=1 || fish2[1] !=0){
            fish2[0] = fish2[0] - 1;
            if(fish2[0]<0){
              fish2[0] = 3; 
            }
          }
        }
        
        else if (randFishDir2== DOWN){
          if(fish2[0]!=3 || fish2[1] !=0){
            fish2[0] = fish2[0] + 1;
            if(fish2[0]>3){
              fish2[0] = 0;
            }
          }
        }
        
        else if (randFishDir2== RIGHT){
          if(fish2[0]!=0 || fish2[1] !=3){
            fish2[1] = fish2[1] + 1;
            if(fish2[1]>3){
              fish2[1]=0;
            }
          }
        }
        
        else{
          if(fish2[0]!=0 || fish2[1] !=1){
            fish2[1] = fish2[1] - 1;
            if(fish2[1]<0){
              fish2[1]=3;
            }
          }
        }
      }

      if(boat1GO == 1){
        if(boat1[0]!=boat1Target[0]){
          boat1[0] = boat1[0] + 1;
        }
        else if (boat1[1]!=boat1Target[1]){
          boat1[1] = boat1[1] +1;
        }
        else{
          boat1GO = 0;
        }
      }

      if(boat2GO == 1){
        if(boat2[0]!=boat2Target[0]){
          boat2[0] = boat2[0] + 1;
        }
        else if (boat2[1]!=boat2Target[1]){
          boat2[1] = boat2[1] +1;
        }
        else{
          boat2GO = 0;
        }
      }


      
      if(boat1Cap < 1){
        if(boat1[0] >0){
          boat1[0] = boat1[0] -1;
        }
        else if (boat1[1]>0){
          boat1[1] = boat1[1] -1;
        }
        else{
          totalFish = totalFish + cap;
          boat1Cap = cap;
          printf("Total caught fish = %d \n",totalFish);
          boat1GO = 1;
        }
        
      }
      if(boat2Cap < 1){
        if(boat2[0] >0){
          boat2[0] = boat2[0] -1;
        }
        else if (boat2[1]>0){
          boat2[1] = boat2[1] -1;
        }
        else{
          totalFish = totalFish + cap;
          boat2Cap = cap;
          printf("Total caught fish = %d \n",totalFish);
          boat2GO = 1;
        }

      }


      MPI_Cart_rank(cartcomm,fish1,&fish1Rank);
      MPI_Cart_rank(cartcomm,boat1,&boat1Rank);
      MPI_Cart_rank(cartcomm,fish2,&fish2Rank);
      MPI_Cart_rank(cartcomm,boat2,&boat2Rank);
      if(totalFish ==20){
        break;
      }
      printf("FISH 1 POS= %d,%d \n",fish1[0],fish1[1]);
      printf("FISH 2 POS= %d,%d \n",fish2[0],fish2[1]);
      printf("Boat 1 POS= %d,%d \n",boat1[0],boat1[1]);
      printf("Boat 2 POS= %d,%d \n",boat2[0],boat2[1]);

      if(fish1Rank == boat1Rank && boat1Cap>0 && boat1GO != 1){
        boat1Cap = boat1Cap -5;
        fish1Cap = fish1Cap -5;
      }else if(fish1Rank == boat2Rank && boat2Cap>0 && boat2GO != 1){
        boat2Cap = boat2Cap -5;
        fish1Cap = fish1Cap -5;
      }else if (fish2Rank == boat1Rank && boat1Cap>0 && boat1GO != 1){
        boat1Cap = boat1Cap -5;
        fish2Cap = fish2Cap -5;
      }else if(fish2Rank == boat2Rank && boat2Cap>0 && boat2GO != 1){
        boat2Cap = boat2Cap -5;
        fish2Cap = fish2Cap -5;
      }
      
      if(counter<1){
        tilbaka==1;
        break;
      }
      counter= counter - 1;
    }
  }
  MPI_Finalize();

  return 0;
}