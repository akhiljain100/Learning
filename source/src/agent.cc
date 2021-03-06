#include "../include/agent.h"
#include <iomanip>
#include <map>
#include <math.h>
#include <unistd.h>
//#include <math.h>
#include <Eigen/Dense>
using namespace Eigen;

Agent::Agent(const Gridworld& g)
{
        grid = g;
        for (int i = 0; i< grid.no_state; i++)
        { 
          
          z_value.push_back(1);
        }
        g_value = new double*[grid.no_state];
        for(int i = 0; i < grid.no_state; ++i)
          g_value[i] = new double[grid.no_state];
        for (int i = 0; i < grid.no_state; i++)
            {for (int j = 0; j < grid.no_state; j++)
             {
              g_value[i][j] = 0;
             }
            }
         for (int i = 0; i< grid.ROW; i++)
        { 
          for(int j = 0; j< grid.COLUMN; j++)
          {
            if(grid.grid[i][j] == 3)
              g_value[i*grid.COLUMN+j][i*grid.COLUMN+j] = exp(-0);
            else if(grid.grid[i][j] == 0 || grid.grid[i][j] == 2)
               g_value[i*grid.COLUMN+j][i*grid.COLUMN+j] = exp(-1);
            else
               g_value[i*grid.COLUMN+j][i*grid.COLUMN+j] = exp(-5);
          }
        }
       q_value.resize( grid.no_state, vector<double> ( 4, 0.0 ) );
}

double Agent::getReward(int agentstate)
  {
    double reward = 0; 
    int row,column;
      row = (agentstate / grid.COLUMN);
      column =  agentstate -  row* grid.COLUMN ;
    if(grid.grid[row][column]== 3)
          {
             reward= 1 +  kl_value[agentstate];
             cout << "\n Reward for the goal state" << reward;

          }
        else if(grid.grid[row][column]== 2 || grid.grid[row][column]== 0)
         {    reward=1 +  kl_value[agentstate]; 
          //cout << "\n KL Value " <<  kl_value[agentstate];
     }   else
          {
             reward = 5 +  kl_value[agentstate];
          } 
         // cout << "\n Reward for agent state " << agentstate << " is " << reward;
    return reward;
}
std::tuple<int, int> Agent::getAction(int agentState)
{
  //action 0 -- left 
  //action 1 -- right 
  //action 2 -- up 
  //action 3 -- down  
    int randomAction,nextstate;
    bool action =false;
    
    while(!action)
    {
      randomAction = rand() % 4;
      //cout << randomAction << agentState;
      if(randomAction == 0)
      {
        if(agentState-1>=0){
        if(grid.s[agentState][agentState-1].prob_state > 0)
        {
          action = true;
          nextstate=agentState-1;
        }
      }
      }
      else if (randomAction == 1)
      {
        if(agentState+1< grid.no_state){
        if(grid.s[agentState][agentState+1].prob_state > 0)
        {
          action = true;
          nextstate=agentState+1;
        }
      }
      }
      else if(randomAction == 2)
      {
        if(agentState-grid.COLUMN>=0){
        if(grid.s[agentState][agentState-grid.COLUMN].prob_state > 0)
        {
          action = true;
          nextstate= agentState-grid.COLUMN;
        }
        }
      }
      else if(randomAction == 3)
      { 
        if(agentState+grid.COLUMN< grid.no_state){
        if(grid.s[agentState][agentState+grid.COLUMN].prob_state > 0)
        {
          action = true;
          nextstate=agentState+grid.COLUMN;
        }
      }
      }
      else
      {
        cout<< "\nSome error !!!";
      }
    }

    return std::make_tuple(randomAction,nextstate);
}
double Agent::findValidMaxElement(int nextstate, bool max = true)
    { 
      int row,column;
      vector <double> validActions;
      double maxe = 0;
      if(grid.s[nextstate][nextstate+grid.COLUMN].prob_state > 0 && nextstate+grid.COLUMN< grid.no_state)
        {
          validActions.push_back(q_value[nextstate][3]);

        }
        if (grid.s[nextstate][nextstate-grid.COLUMN].prob_state > 0 && nextstate-grid.COLUMN>=0)
        {
          validActions.push_back(q_value[nextstate][2]);
        }
        if (grid.s[nextstate][nextstate+1].prob_state > 0 && nextstate+1< grid.no_state)
        {
          validActions.push_back(q_value[nextstate][1]);
        }
        if(grid.s[nextstate][nextstate-1].prob_state > 0 && nextstate-1>=0)
        {
          validActions.push_back(q_value[nextstate][0]);
        }
        row = (nextstate / grid.COLUMN);
        column =  nextstate -  row* grid.COLUMN ;
        if(grid.grid[row][column] != 3 )
        {
          
      auto it = min_element(begin(validActions), end(validActions));
      maxe=*it;
      }
      else{
        maxe=0;
      }  
      return maxe;
}
int Agent::findActionMaxElement(int nextstate)
    { 
      vector <int> validActions;
      int row,column;
      double maxe = 0;
      
        if(grid.s[nextstate][nextstate+grid.COLUMN].prob_state > 0 && nextstate+grid.COLUMN< grid.no_state)
        {
          validActions.push_back(3);


        }
        if (grid.s[nextstate][nextstate-grid.COLUMN].prob_state > 0 && nextstate-grid.COLUMN>=0)
        {
          validActions.push_back(2);
        }
        if (grid.s[nextstate][nextstate+1].prob_state > 0 && nextstate+1< grid.no_state)
        {
          validActions.push_back(1);
        }
        if(grid.s[nextstate][nextstate-1].prob_state > 0 && nextstate-1 >=0)
        {
          validActions.push_back(0);
        }
        /*for(int i = 0; i < validActions.size(); i++)
        {
          cout << "\n" << validActions[i];
        }*/
       int index = 0;
        row = (nextstate / grid.COLUMN);
        column =  nextstate -  row* grid.COLUMN ;
        /*if(nextstate == 0)
        { cout << "\n" << grid.no_state;
          cout << "\n Actions " ;
          for(int op = 0; op < validActions.size(); op++)
            cout << "\n" <<validActions[op];
        }*/
        if(grid.grid[row][column] != 3)
        {   for(int i = 1; i < validActions.size(); i++)
            {
                if(q_value[nextstate][validActions[i]] < q_value[nextstate][validActions[index]])
                    index = i;              
            }
      }
      else{
        index = 4;
      } 

      return validActions[index];
}
std::tuple<int, int> Agent::getGreedyAction(int agentState)
{
  //action 0 -- left 
  //action 1 -- right 
  //action 2 -- up 
  //action 3 -- down 
    int randomAction=0,nextstate;
    bool action =false;
    //double epsilon = 0.9;
    double genRandomNum = ((double) rand() / (RAND_MAX));
    //genrateRandomNum = rand();
    if(epsilon <= genRandomNum  )
    {
    while(!action)
    {
      randomAction = rand() % 4;
      if(randomAction == 0)
      {
        if(agentState-1>=0){
        if(grid.s[agentState][agentState-1].prob_state > 0)
        {
          action = true;
          nextstate=agentState-1;
        }
      }
      }
      else if (randomAction == 1)
      {
        if(agentState+1< grid.no_state){
        if(grid.s[agentState][agentState+1].prob_state > 0)
        {
          action = true;
          nextstate=agentState+1;
        }
      }
      }
      else if(randomAction == 2)
      {
        if(agentState-grid.COLUMN>=0){
        if(grid.s[agentState][agentState-grid.COLUMN].prob_state > 0)
        {
          action = true;
          nextstate= agentState-grid.COLUMN;
        }
        }
      }
      else if(randomAction == 3)
      { 
        if(agentState+grid.COLUMN< grid.no_state){
        if(grid.s[agentState][agentState+grid.COLUMN].prob_state > 0)
        {
          action = true;
          nextstate=agentState+grid.COLUMN;
        }
      }
      }
      else
      {
        cout<< "\nSome error !!!";
      }
    }
    }
    else 
    {
      //cout << "\n Inside Greedy Action Unit";
      randomAction = findActionMaxElement(agentState);
      //cout << "\n Agent State " << agentState << " action " << randomAction;
      nextstate=0;

          if(randomAction == 0)
          {
            nextstate=agentState-1;          
          }
          else if(randomAction == 1){
            nextstate=agentState+1;
          }
          else if(randomAction == 2){
            nextstate= agentState-grid.COLUMN;
          }
          else if(randomAction == 3)
          {
            nextstate=agentState+grid.COLUMN;
          }
          else 
          {
            nextstate = 0;
            randomAction=1;
          }
          //cout << "\n Greedy unit next state " << nextstate;
      }
    return std::make_tuple(randomAction,nextstate);
}
void Agent::greedyQlearning()
{ q_value.resize( grid.no_state, vector<double> ( 4, 0.0 ) );
  
  for(int n = 0 ; n < num_iter; n++)
  {
    for (int i = 0 ; i < q_value.size() ; i++)
  {
    for(int j = 0; j< q_value[i].size(); j++)
    {
      q_value[i][j] = 0.0;
    }
  }
      double tot_step=0,step = 0,reward,alpha,gamma=0.95,maxElement;
      
      int row, column, agentState,action,nextstate=0;
      vector <double> q_value_state;
      vector <double> q_value_step;
      vector<vector<double> > qvalueAftereachstep;
      while(tot_step < tot_steps)
      {
          step = 0;
          agentState = grid.start_state[0][0]*grid.COLUMN+grid.start_state[0][1];
          row = (agentState / grid.COLUMN);
          column =  agentState -  row* grid.COLUMN ;
          while (grid.grid[row][column] != 3 && tot_step < tot_steps)
          {
                  std::tie(action, nextstate) = getGreedyAction(agentState);
                  //cout << "\n Agent state " << agentState << "Next state" << nextstate;
                  alpha =  (double)randomAlpha/(double)(randomAlpha+tot_step);
                  reward = getReward(nextstate); 
                  maxElement = findValidMaxElement(nextstate);
                  if(agentState == 11)
                  {
                    cout << "\n Action " << action << "Max Element " << maxElement << "Reward" << reward;
                  }
                  q_value[agentState][action] = (1-alpha)*q_value[agentState][action] + alpha * (reward+ gamma * maxElement);

                  agentState = nextstate;
                  row = (agentState / grid.COLUMN);
                  column =  agentState -  row* grid.COLUMN ;
                  
                  step = step + 1;
                  tot_step = tot_step +1;

                   for(int i=0;i<grid.ROW;i++)
                {

                  for(int j=0;j<grid.COLUMN;j++)
                  {
                      if(grid.grid[i][j] != 1 )
                    {
                 maxElement = findValidMaxElement(i*grid.COLUMN+j,true);
                 }
                 else {
                  maxElement = 0;
                 }
                 q_value_step.push_back(maxElement);

                }
              
              } 
               qvalueAftereachstep.push_back(q_value_step);
                q_value_step.clear();

          }
      
      }
     
      std::ofstream outfileQv ("../Result/greedyq_valueafterstep" + to_string(n) + ".dat", std::ofstream::out);
      cout << "\n" << qvalueAftereachstep.size();
      for(int i=0;i<qvalueAftereachstep.size();i++){
       
      for(int j=0;j<qvalueAftereachstep[i].size();j++){
        
          outfileQv <<qvalueAftereachstep[i][j] << "  " ;
        }
      outfileQv << "\n";
     // cout << "\n";
      }
      outfileQv.close();
      ofstream outfile("../Result/greedyqnew_iteration" + to_string(n) + ".dat");
        
        for(int i=0;i<grid.ROW;i++)
        {

          for(int j=0;j<grid.COLUMN;j++)
          {
            if(grid.grid[i][j] != 1 )
            {
         maxElement = findValidMaxElement(i*grid.COLUMN+j,true);
         }
         else {
          maxElement = 0;
         }

             outfile << maxElement << " ";
          }
          outfile << "\n";
        }
        outfile.close();
  }

}


void Agent::randomQlearning()
{

 for(int n = 0 ; n < num_iter; n++)
  {
    for (int i = 0 ; i < q_value.size() ; i++)
  {
    for(int j = 0; j< q_value[i].size(); j++)
    {
      q_value[i][j] = 0.0;
    }
  }
      double tot_step=0,step = 0,reward,alpha,gamma=0.95,maxElement;
      
      int row, column, agentState,action,nextstate=0;
      vector <double> q_value_state;
      vector <double> q_value_step;
       vector<vector<double> > qvalueAftereachstep;
      while(tot_step < tot_steps)
      {
          step = 0;
          agentState = grid.start_state[0][0]*grid.COLUMN+grid.start_state[0][1];
          row = (agentState / grid.COLUMN);
          column =  agentState -  row* grid.COLUMN ;
          while (grid.grid[row][column] != 3 && tot_step < tot_steps)
          {
          
          
          
                  std::tie(action, nextstate) = getAction(agentState);
                  alpha =  (double)randomAlpha/(double)(randomAlpha+tot_step);
                  reward = getReward(nextstate);	
                  maxElement = findValidMaxElement(nextstate);
                  q_value[agentState][action] = (1-alpha)*q_value[agentState][action] + alpha * (reward+ gamma * maxElement);

                  agentState = nextstate;
              	  row = (agentState / grid.COLUMN);
                  column =  agentState -  row* grid.COLUMN ;
                  
                  step = step + 1;
                  tot_step = tot_step +1;

                   for(int i=0;i<grid.ROW;i++)
                {

                  for(int j=0;j<grid.COLUMN;j++)
                  {
                      if(grid.grid[i][j] != 1 )
                    {
                 maxElement = findValidMaxElement(i*grid.COLUMN+j,true);
                 }
                 else {
                  maxElement = 0;
                 }
                 q_value_step.push_back(maxElement);

                }
              
              } 
               qvalueAftereachstep.push_back(q_value_step);
                q_value_step.clear();

          }
      
      }
     
      std::ofstream outfileQv ("../Result/randomq_valueafterstep" + to_string(n) + ".dat", std::ofstream::out);
      //ofstream outfileQv("../Result/randomq_value.txt");
      cout << "\n" << qvalueAftereachstep.size();
      for(int i=0;i<qvalueAftereachstep.size();i++){
       
      for(int j=0;j<qvalueAftereachstep[i].size();j++){
        
          outfileQv <<qvalueAftereachstep[i][j] << "  " ;
        }
      outfileQv << "\n";
      }
      outfileQv.close();
      ofstream outfile("../Result/randomqnew_iteration" + to_string(n) + ".dat");
      for(int i=0;i<grid.ROW;i++)
      	{

      		for(int j=0;j<grid.COLUMN;j++)
      		{
            if(grid.grid[i][j] != 1 )
            {
         maxElement = findValidMaxElement(i*grid.COLUMN+j,true);
         }
         else {
          maxElement = 0;
         }

             outfile << maxElement << " ";
      		}
      		outfile << "\n";
      	}
      	outfile.close();
  }


  }
void Agent::valueIteration()

{

  int row, column, agentState,step,nextstate;
  double tot_step = 0;
  double valueMatrix[grid.no_state];
  for(int i=0;i<grid.no_state;i++)
    {
                valueMatrix[i]=0;
    }
  double alpha;
  vector<int> nextState;
  vector<double> nextPossibleState;
  //valueMatrix[120] = 10;
  int tot_steps=100000;
  while(tot_step < tot_steps)
  {
      step = 0;
      agentState = grid.start_state[0][0]*grid.COLUMN+grid.start_state[0][1];
      row = (agentState / grid.COLUMN);
      column =  agentState -  row* grid.COLUMN ;
      while (step < steps && grid.grid[row][column] != 3 && tot_step < tot_steps)
      {
        nextPossibleState.clear();
        nextState.clear();
        row = (agentState / grid.COLUMN);
        column =  agentState -  row* grid.COLUMN ;
        for(int i=0;i<grid.no_state;i++)
        {
          if(grid.s[agentState][i].prob_state > 0)
          {
            
           nextPossibleState.push_back(valueMatrix[i]*0.95);
    
           nextState.push_back(i);
          }
        }
        nextstate = nextState[rand() % nextState.size()];
        auto it = min_element(begin(nextPossibleState), end(nextPossibleState));
        double reward=0;
        if(grid.grid[row][column]== 3)
          {
             reward= 0 + kl_value[agentState];
          }
        else if(grid.grid[row][column]== 2 || grid.grid[row][column]== 0)
             reward=1 + kl_value[agentState]; 
        else
          {
             reward = 5 + kl_value[agentState];
          } 
          cout << "\n Reward for agent state in value iteration case "  << agentState << " is " << reward;
         
        valueMatrix[agentState]= reward+ (*it);
        nextPossibleState.clear();
        agentState = nextstate;
        step = step + 1;
        tot_step = tot_step + 1;
      }
  
  }
  ofstream outfile("../Result/v_iteration.dat");
  for(int i=0;i<grid.ROW;i++){

  for(int j=0;j<grid.COLUMN;j++){
      outfile << valueMatrix[i*grid.COLUMN+j] << " ";
  }
  outfile << "\n";
  }
  outfile.close();

}
void Agent::learnAgent()
{
    double dotmatrix[grid.no_state][grid.no_state];
    /*cout << "\n\n G Matrix exp(-q(i)) \n\n";
     for (int i = 0; i < grid.no_state; i++)
      {
        cout << " ";
        for (int j = 0; j < grid.no_state; j++)
        {
          cout << "-------";
          }
          cout << "\n";
          
          
        for (int j = 0; j < grid.no_state; j++)
        {
          if(j==0)cout << "| ";
          size_t save_prec = cout.precision();
          cout << fixed << setfill ('0')<< setprecision(2) << g_value[i][j] << " | ";
          cout.precision(save_prec);
        }
        cout << "\n";
        if(i+1 == grid.no_state)
        {
          cout << " ";
          for (int j = 0; j < grid.no_state; j++)
        {cout << "-------";
          }
        }
        
      }*/
    for(int i=0;i<grid.no_state;i++){
        for(int j=0;j<grid.no_state;j++){
                dotmatrix[i][j]=0;
                for(int k=0;k<grid.no_state;k++){
                  dotmatrix[i][j]+=(g_value[i][k] * grid.s[k][j].prob_state);
            }
        }
      }
      /*
      cout << "\n\n GP Matrix \n\n";
         for (int i = 0; i < grid.no_state; i++)
          {
            cout << " ";
            for (int j = 0; j < grid.no_state; j++)
            {
              cout << "-------";
              }
              cout << "\n";
              
              
            for (int j = 0; j < grid.no_state; j++)
            {
              if(j==0)cout << "| ";
              cout << fixed << setfill ('0')<< setprecision(2) << dotmatrix[i][j] << " | ";
            }
            cout << "\n";
            if(i+1 == grid.no_state)
            {
              cout << " ";
              for (int j = 0; j < grid.no_state; j++)
            {cout << "-------";
              }
            }
            
          }
      */

      double temp_z = 0.0;

      vector<double> vector_z;
      for (int n= 0; n< no_Ziter; n++){
        //cout << "\n";
        z_value = vector_z;
        vector_z.clear();
       for(int i=0;i<grid.no_state;i++){
        temp_z=0.0;
      for(int j=0;j<grid.no_state;j++){
             
      temp_z +=z_value[j]*dotmatrix[i][j];

      }
      vector_z.push_back(temp_z);
      }
      
      }
      
      //Allocate memory to Control trajectory of the grid
      cs = (controlled_state **) malloc(sizeof(controlled_state *) * grid.no_state);
          for(int i = 0; i < grid.no_state; i++) 
          {
            cs[i] = (controlled_state *) malloc(sizeof(controlled_state) * grid.no_state);
          }
   for(int k = 0; k < grid.no_state; k++)
   {
      double norm = 0;
      //finding out the next possible states and also calculating the normalization value
      
      for(int i=0;i<grid.no_state;i++)
      {

        if(grid.s[k][i].prob_state > 0)
        {
          if(k == 0 )
          {
            cout << "\n Z value for " << i << " state " << z_value[i];
            cout << "\n" << grid.s[k][i].prob_state ;
          }
          norm += (grid.s[k][i].prob_state * z_value[i]);
        }
         if(k == 0 )
          {
        cerr << "\nNorm value for 1st state " << norm;
      }
      }


      //Calculating the control probability 
       for (int j = 0; j < grid.no_state; j++)
            {
              if(grid.s[k][j].prob_state!= 0)
                {
                  cs[k][j].control_prob =   double(grid.s[k][j].prob_state * double(z_value[j]/double(norm)));
                }
              else
                {
                cs[k][j].control_prob = 0;
                }
            }
    }
        cout << "\n\n Control prob Matrix \n\n";
         for (int i = 0; i < grid.no_state; i++)
          {
            cout << " ";
            for (int j = 0; j < grid.no_state; j++)
            {
              cout << "-------";
              }
              cout << "\n";
              
              
            for (int j = 0; j < grid.no_state; j++)
            {
              if(j==0)cout << "| ";
              cout << fixed << setfill ('0')<< setprecision(2) << cs[i][j].control_prob << " | ";
            }
            cout << "\n";
            if(i+1 == grid.no_state)
            {
              cout << " ";
              for (int j = 0; j < grid.no_state; j++)
            {cout << "-------";
              }
            }
            
          }
}



void Agent::saveZIterationValue()
{

  ofstream outfile("../Result/z_iteration.dat");
  for(int i=0;i<grid.ROW;i++){

  for(int j=0;j<grid.COLUMN;j++){
      outfile << -log(z_value[i*grid.COLUMN+j]) << " ";

  }
  outfile << "\n";
  }
  outfile.close();
}



void Agent::trainZlearning()
{ 
  for(int n = 0 ; n < num_iter; n++)
  {
    z_value.clear();
    for (int i = 0; i< grid.no_state; i++)
    { 
      z_value.push_back(1);
    }
  double tot_step=0,alpha;
  int row, column, agentState,nextstate = 0,step=0;
  vector<int> nextPossibleState;
  vector<vector<double> > zvalueAftereachstep;
  while(tot_step < tot_steps)
  {
   // Recomputing from the start state, right now we are just using the first start state
   // Todo: computing with random start state, expecting interesting result 
      step = 0;
      agentState = grid.start_state[0][0]*grid.COLUMN+grid.start_state[0][1];
      row = (agentState / grid.COLUMN);
      column =  agentState -  row* grid.COLUMN ;
      //cout << steps <<grid.grid[row][column]<<step;
      // Computing the z_value for state in each step based on the next state agent takes
      while (step < steps && grid.grid[row][column] != 3 && tot_step < tot_steps)
      {
      row = (agentState / grid.COLUMN);
      column =  agentState -  row* grid.COLUMN ;
      //cout<<"no of step" << step << "\n\n";

      
      double rand_no = (double)rand() / RAND_MAX ;
      int i = 0;
      double prob_m = 0;
      while(rand_no > prob_m )
        {
          prob_m += grid.s[agentState][i].prob_state;
          i++;
        }

      nextstate = --i;
      alpha =  (double)calpha/(double)(calpha+tot_step);

      z_value[agentState] = (1-alpha)*z_value[agentState] + alpha* g_value[agentState][agentState]* z_value[nextstate];
      zvalueAftereachstep.push_back(z_value);
      agentState = nextstate;
      step = step + 1;
      tot_step = tot_step +1;
      }

     
  }
  string file = "../Result/randomz_valueaftereachstep" + to_string(n) + ".dat";
  
  ofstream outfileZvalue(file);
  for(int i=0;i<zvalueAftereachstep.size();i++){

  for(int j=0;j<zvalueAftereachstep[i].size();j++){
    row = (j / grid.COLUMN);
  column =  j -  row* grid.COLUMN ;
  if(grid.grid[row][column] == 1)
  {
    outfileZvalue << -log(0) << " ";
  }
  else{
      outfileZvalue << -log(zvalueAftereachstep[i][j]) << " ";
      }
  }

  outfileZvalue << "\n";
  }
  string filez = "../Result/randomz_zvalueaftertotsteps" + to_string(n) + ".dat";
  ofstream outfile(filez);
  for(int i=0;i<grid.ROW;i++){

  for(int j=0;j<grid.COLUMN;j++){
  	
      outfile << -log(z_value[i*grid.COLUMN+j]) << " ";
  }
  outfile << "\n";
  }
  outfile.close();
  outfileZvalue.close();
  cout << tot_steps;
}
}
void Agent::calKLDivergence()
{
   
  for (int i = 0; i< grid.no_state; i++)
  {
    double norm = 0;
    int n = 0;
    for(int j = 0;j< grid.no_state; j++)
    {
      if(grid.s[i][j].prob_state!=0)
      {
        if(i == 0)
        {
          cerr << "\n COntorl prob for " << i << " state to " << j << " state is " << cs[i][j].control_prob ;
       //   cout << "\n passive prob for " << i << " state "<< grid.s[i][j].prob_state;
        }
      norm +=  cs[i][j].control_prob *log(cs[i][j].control_prob /grid.s[i][j].prob_state);
      n= n+1;
      }
      //cout << "\nnorm value " << norm; Agent behaves near passive dynamics kl divergence gets small 
    }
    kl_value.push_back(norm/n);
  }
  for (int i =0 ; i< kl_value.size();i++)
  {
    cout << "\nkl value for " << i << " state is " << kl_value[i];
  }

}
void Agent::greedyZlearning()
{
  for(int n = 0 ; n < num_iter; n++)
  {
        // Reinitialize Z value as it may have been changed by other functions
       z_value.clear();
        for (int i = 0; i< grid.no_state; i++)
        { 
          
          z_value.push_back(1);
        }

    //Allocate memory to Control trajectory of the grid
      cs = (controlled_state **) malloc(sizeof(controlled_state *) * grid.no_state);
          for(int i = 0; i < grid.no_state; i++) 
          {
            cs[i] = (controlled_state *) malloc(sizeof(controlled_state) * grid.no_state);
          }

       // Local variables for the manipulation of the agent    
      int nextstate = 0;

      double tot_step=0;
      int row, column, agentState;
      vector<int> nextPossibleState;
      double alpha;
      int step = 0;
      double norm;
      vector<vector<double> > zvalueAftereachstep;

      while(tot_step < tot_steps)
      {
       // Recomputing from the start state, right now we are just using the first start state
       // Todo: computing with random start state, expecting interesting result 
      step = 0;
      agentState = grid.start_state[0][0]*grid.COLUMN+grid.start_state[0][1];
      row = (agentState / grid.COLUMN);
      column =  agentState -  row* grid.COLUMN ;


      // Computing the z_value for state in each step based on the next state agent takes
      while (step < steps && grid.grid[row][column] != 3 && tot_step < tot_steps)
      {
      row = (agentState / grid.COLUMN);
      column =  agentState -  row* grid.COLUMN ;
      //cout<<"no of step" << step << "\n\n";
      // after each step, z value is getting changed and so the norm value as well
      norm = 0;
      //finding out the next possible states and also calculating the normalization value
      for(int i=0;i<grid.no_state;i++)
      {

        if(grid.s[agentState][i].prob_state > 0)
        {
          nextPossibleState.push_back(i);
          norm += (grid.s[agentState][i].prob_state * z_value[i]);
        }
      }


      //Calculating the control probability 
       for (int j = 0; j < grid.no_state; j++)
            {
              if(grid.s[agentState][j].prob_state!= 0)
                {
                  cs[agentState][j].control_prob =   double(grid.s[agentState][j].prob_state * double(z_value[j]/double(norm)));
                }
              else
                {
                cs[agentState][j].control_prob = 0;
                }
            }
      /*
      for(int k= 0; k< nextPossibleState.size(); k++)
      {
        cout << nextPossibleState[k] << "state possible \n";
      }
      vector <double> para_control_prob;
      for(int i=0;i<grid.no_state;i++)
      {
        para_control_prob.push_back(cs[agentState][i].control_prob);
      }
      nextstate = Sample(para_control_prob);
      */

      //Sampling based on control probability. Take the random number and sample the control prob according to the random number generated.
      //For ex. if the control prob distribution is 0.1 0.8 0.1 for the next states, and the random number we got say 0.5 then it lies in the range of second control prob i.e, 0.8
      //as its range is from 0.1 to 0.9 and thus we select 2nd state but if we get value greater than 0.9 or less than 0.1 then we select the other two states.
      
      double rand_no = (double)rand() / RAND_MAX ;
      int i = 0;
      double prob_m = 0;
      while(rand_no > prob_m )
        {
          prob_m += cs[agentState][i].control_prob;
          i++;
        }

      nextstate = --i;
     /* double max = cs[agentState][0].control_prob;
      int k = 0;
      for(int i  = 1; i < grid.no_state; i++)
      {
        if(max < cs[agentState][i].control_prob)
        {
          max = cs[agentState][i].control_prob;
          k = i;
        }

      }
      nextstate = k;*/
      /*if(episode == 0)
            {cerr<<"\n probability sum"<< prob_m << "\n";
            cout <<"\nagentstate" <<  agentState << "nextstate"<< nextstate << "\n"; 
            cerr <<"\n" << rand_no << "\n";
          }
        */
      // Random selection of the next state 
      //nextstate = nextPossibleState[rand() % nextPossibleState.size()];

      // Clearing the vector so that we can reuse this vector for next states
      nextPossibleState.clear();

      alpha =  (double)calpha/(double)(calpha+tot_step);

      //cerr << "\nalpha " << alpha << "\n";
      //Update the Z value by importance sampling method where we try to sample the passive dynamics according to the control probability distribution
      z_value[agentState] = (1-alpha)*z_value[agentState] + alpha* g_value[agentState][agentState]* z_value[nextstate] * double(grid.s[agentState][nextstate].prob_state/cs[agentState][nextstate].control_prob);
      // Jump to next state
      agentState = nextstate;

      zvalueAftereachstep.push_back(z_value);
      step = step + 1;
      tot_step = tot_step +1;
      }
      //cout << "\n\n" << "no of episode " << episode << "\n\n";

      /*for(int i=0;i<grid.no_state;i++)
      {
        cerr << "        " << z_value[i];
      }
      cout << "\n\n";*/
      }


        string file = "../Result/greedyz_zvalueaftereachsteps" + to_string(n) + ".dat";
      // This loop stores the z_value after each episode into the text file

      ofstream outfileZvalue(file);
      for(int i=0;i<zvalueAftereachstep.size();i++){

      for(int j=0;j<zvalueAftereachstep[i].size();j++){
        row = (j / grid.COLUMN);
      column =  j -  row* grid.COLUMN ;
      if(grid.grid[row][column] == 1)
      {
        outfileZvalue << -log(0) << " ";
      }
      else{
          outfileZvalue << -log(zvalueAftereachstep[i][j]) << " ";
          }
      }

      outfileZvalue << "\n";
      }


        string filez = "../Result/greedyz_zvalueaftertotsteps" + to_string(n) + ".dat";
      // to store the final z_value in the file 
      ofstream outfile(filez);
      for(int i=0;i<grid.ROW;i++){

      for(int j=0;j<grid.COLUMN;j++){
          outfile << -log(z_value[i*grid.COLUMN+j]) << " ";
      }
      outfile << "\n";
      }
      outfile.close();
      outfileZvalue.close();

      }


}
