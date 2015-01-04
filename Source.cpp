#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<queue>
#include<stack>
using namespace std;

//Get all inputs in one object



class node 
{
public:
	int id;
	int input_id;
	string	state;
	int parent;
	int depth;
	string	parent_name;
	int path_cost;
	node ()
	{
		id = -1;
		state = "";
		parent = -1;
		depth = -1;
		input_id=-1;
		parent_name="";
		path_cost=0;
	}
};

class agent_input
{
public:
	int algo_type;
	vector<string> endpoints;
	int number_of_participants;
	vector<node> participant_name;
	int **edges;

	void initialize()
	{
		ifstream infile;
		string line;
		int i,j;
		infile.open("input.txt");
		
		infile >> line;
		algo_type = atoi(line.c_str());  //type of algo

		infile >> line;
		endpoints.push_back(line);  //source

		infile >> line;
		endpoints.push_back(line);  //destination

		infile >> line;
		number_of_participants = atoi(line.c_str());  //type of algo

		for (i = 0; i < number_of_participants; i++ )
		{
			infile >> line;
			node p;
			p.parent=-1;
			p.depth=-1;
			p.input_id=i;
			p.state=line;
			participant_name.push_back(p);
		}

		edges = new int*[number_of_participants];
		for(i = 0; i < number_of_participants; i++)
			edges[i] = new int[number_of_participants];

		for(i = 0; i < number_of_participants; i++)
		{
			for(j = 0; j < number_of_participants; j++)
			{
				infile >> line;
				edges [i][j] = atoi(line.c_str());
			}
		}

	}

	void display ()
	{
		cout << "algo_type: " << algo_type;
		cout << "\n";
		cout << "Source: " << endpoints[0];
		cout << "\n";
		cout << "Destination: " << endpoints[1];
		cout << "\n";
		cout << "number_of_participants: " << number_of_participants;
		cout << "\n";
		for (int i=0;i <  number_of_participants;i++)
			cout << "participant: "  << participant_name[i].input_id <<":" << participant_name[i].state <<"\n";

		for(int i = 0; i < number_of_participants; i++)
		{
			for(int j = 0; j < number_of_participants; j++)
			{
				cout << edges[i][j] << "   ";
			}
			cout << "\n";
		}
		
	}

};

struct sortAlphabeticallyBFS {
    bool operator()(node const & p1, node const & p2) {
        // return "true" if "p1" is ordered before "p2", for example:
		if (p1.depth < p2.depth)
			return false;
		else if (p1.depth > p2.depth)
			return true;
		else if (p1.depth == p2.depth)
		{
		   if (p1.state < p2.state)
			   return false;
		   else if (p1.state > p2.state)
			   return true;
		   else if (p1.state==p2.state)
		   {
			   if (p1.parent_name<p2.parent_name)
				   return false;
			   else if (p1.parent_name>p2.parent_name)
				   return true;
		   }
		}
    }
};

struct sortAlphabeticallyDFS {
    bool operator()(node const & p1, node const & p2) {
        // return "true" if "p1" is ordered before "p2", for example:
		if (p1.depth < p2.depth)
			return true;
		else if (p1.depth > p2.depth)
			return false;
		else if (p1.depth == p2.depth)
		{
		   if (p1.state < p2.state)
			   return false;
		   else if (p1.state > p2.state)
			   return true;
		   else if (p1.state==p2.state)
		   {
			   if (p1.parent_name<p2.parent_name)
				   return false;
			   else if (p1.parent_name>p2.parent_name)
				   return true;
		   }
		}
    }
};

struct sortAlphabeticallyUCS {
    bool operator()(node const & p1, node const & p2) {
        // return "true" if "p1" is ordered before "p2", for example:
		if (p1.path_cost < p2.path_cost)
			return false;
		else if (p1.path_cost > p2.path_cost)
			return true;
		else if (p1.path_cost == p2.path_cost)
		{
		   if (p1.state < p2.state)
			   return false;
		   else if (p1.state > p2.state)
			   return true;
		   else if (p1.state==p2.state)
		   {
			   if (p1.parent_name<p2.parent_name)
				   return false;
			   else if (p1.parent_name>p2.parent_name)
				   return true;
		   }
		}
    }
};


class algorithm
{
public: 
		agent_input input;
		int *visited;
		int *presentInQueue;
		priority_queue<node, vector<node>, sortAlphabeticallyBFS> in_queueBFS;
		priority_queue<node, vector<node>, sortAlphabeticallyDFS> in_queueDFS;
		priority_queue<node, vector<node>, sortAlphabeticallyUCS> in_queueUCS;
//	queue<node> in_queueBFS;
		queue<node> exp_queue;
		queue<node> out_queue;

int getInputID(string iState)
{
//	cout << "Inside getInputID "<<"\n";
	int x=0;
	string str="";
				while (true)
				{
	//				cout << "Inside getInputID loop"<<"\n";
					str=input.participant_name[x].state;
		//			cout << "Participant  "<<str<<"\n";
					if(str.compare(iState) == 0)
						return x;
					else
						x++;
				}
				
}

void printResults(node status)
{
	vector<node> out_nodes;
	vector<int> compute_cost;
	int x,y;
	ofstream outfile;
	outfile.open("output.txt");
	int cul_cost=0;
	stack<string> node_name;
	if (status.state=="")
	{
		int size = exp_queue.size();
		int count = 0;
		while (!exp_queue.empty())
		{
			node n=exp_queue.front();
			exp_queue.pop();
			outfile << n.state;
			count ++;
			//if (i != out_queue.size() -1)
			if (count!=size)
				outfile << "-";
		}
		outfile << "\n";
		outfile << "NoPathAvailable";
	}
	else
	{
		int size = exp_queue.size();
		int count = 0;
		while (!exp_queue.empty())
		{
			node n=exp_queue.front();
			exp_queue.pop();
			outfile << n.state;
			count ++;
			//if (i != out_queue.size() -1)
			if (count!=size)
				outfile << "-";
		}
		outfile << "\n";
		while (!out_queue.empty())
		{
			node n=out_queue.front();
			out_queue.pop();
			out_nodes.push_back(n);
		}
//		cout<<"-------------------------------"<<status.path_cost;
//		cout <<"\n";
		string str;
		str=status.state;
		node_name.push(str);
		compute_cost.push_back(status.input_id);
		int status_id=status.parent;
		while(status_id != -1)
		{
			for (int i=0;i <out_nodes.size();i++)
			{
				if (out_nodes[i].id==status_id)
				{
					str=out_nodes[i].state;
					node_name.push(str);
					compute_cost.push_back(out_nodes[i].input_id);
					status_id=out_nodes[i].parent;
				}
			}

		}
		int stk_size=node_name.size();
		while (!node_name.empty())
		{
			str=node_name.top();
			node_name.pop();
			outfile << str;
			stk_size--;
			if (stk_size!=0)
				outfile << "-";
		}
		outfile << "\n";
/*		for (int i=0;i < compute_cost.size()-1;i++)
		{    
			x=compute_cost[i];
			y=compute_cost[i+1];
			cul_cost+=input.edges[x][y];
			//cout <<"x: "<<x<<" y: "<<y<<"\n"; 
			//cout <<"cul_cost: " << cul_cost <<" + "<<input.edges[x][y];
		}*/
		outfile << status.path_cost;

	}
	
}

void checkQueueBFS(node n)
{
	vector<node> nodeContainer;
	
	node inVectorNode;
	int size=in_queueBFS.size();
	while (!in_queueBFS.empty())
		{
			node tmp=in_queueBFS.top();
			in_queueBFS.pop();
			nodeContainer.push_back(tmp);
			
		}
	for(int i=0;i<size;i++)
	{
		inVectorNode=nodeContainer.at(i);
		
		if (inVectorNode.state!=n.state)
			in_queueBFS.push(inVectorNode);
		else
		{
			if (n.depth<inVectorNode.depth)
				in_queueBFS.push(n);
			else
				in_queueBFS.push(inVectorNode);

		}

	}


}

void checkQueueDFS(node n)
{
	vector<node> nodeContainer;
	
	node inVectorNode;
	int size=in_queueDFS.size();
	while (!in_queueDFS.empty())
		{
			node tmp=in_queueDFS.top();
			in_queueDFS.pop();
			nodeContainer.push_back(tmp);
			
		}
	for(int i=0;i<size;i++)
	{
		inVectorNode=nodeContainer.at(i);
		
		if (inVectorNode.state!=n.state)
			in_queueDFS.push(inVectorNode);
		else
		{
			if (n.depth<inVectorNode.depth)
				in_queueDFS.push(n);
			else
				in_queueDFS.push(inVectorNode);

		}

	}


}

void checkQueueUCS(node n)
{
	vector<node> nodeContainer;
	
	node inVectorNode;
	int size=in_queueUCS.size();
	while (!in_queueUCS.empty())
		{
			node tmp=in_queueUCS.top();
			in_queueUCS.pop();
			nodeContainer.push_back(tmp);
			
		}
	for(int i=0;i<size;i++)
	{
		inVectorNode=nodeContainer.at(i);
		
		if (inVectorNode.state!=n.state)
			in_queueUCS.push(inVectorNode);
		else
		{
			if (n.path_cost<inVectorNode.path_cost)
				in_queueUCS.push(n);
			else
				in_queueUCS.push(inVectorNode);

		}

	}


}


node bfs()
{
	//cout << "inside algorithm bfs"<<"\n";
	//input.display();
	int node_number=1;
	int depth_number=0;
	//int *visited;
	visited = new int [input.number_of_participants];
	presentInQueue = new int [input.number_of_participants];
	//cout << "created visited"<<"\n";
	for (int i=0;i<input.number_of_participants;i++)
		visited[i]=0;
	//cout << "initialized visited"<<"\n";
	for (int i=0;i<input.number_of_participants;i++)
		presentInQueue[i]=0;
	node p;
	node n,temp;
	string source=input.endpoints[0];
	string destination=input.endpoints[1];
	//cout << "Got source and destination"<<"\n";
	n.id=node_number;
	node_number++;
	n.state=source;
	n.input_id=getInputID(source);
	n.depth=depth_number;
	n.parent=-1;
	n.parent_name="";
	n.path_cost=0;
	if (presentInQueue[n.input_id]==0)
	{
		in_queueBFS.push(n);
		presentInQueue[n.input_id]=1;
	}
	else if(presentInQueue[n.input_id]==1)
	{
		checkQueueBFS(n);
	}

	//exp_queue.push(n);
	//visited[n.input_id]=1;
	//cout << "inside algorithm bfs"<<"\n";
	while (true)
	{
		if (in_queueBFS.empty())
		{
			node ret;
			ret.state="";
			return ret;
		}
		else
		{
			temp = in_queueBFS.top();
			//temp = in_queueBFS.front();
			in_queueBFS.pop();
			//out_queue.push(temp);
			//exp_queue.push(temp);
			//visited[temp.input_id]=1;
		//	cout << "processing: " <<temp.id<<"--->" <<temp.state<<"-->"<<temp.parent<<"-->"<<temp.parent_name<<"\n";
			if (temp.state == destination)
			{
				exp_queue.push(temp);
				return temp;
			}
			else
			{
				//depth_number++;
				int x = getInputID(temp.state);
				for (int i=0;i<input.number_of_participants;)
				{
					
					if (input.edges[x][i]==0 || visited[i]==1)
						i++;
					else 
					{
						
						p.id=node_number;
						node_number++;
						p.parent=temp.id;
						p.depth=temp.depth+1;
						p.state=input.participant_name[i].state;
						p.input_id=input.participant_name[i].input_id;
						p.parent_name=temp.state;
						p.path_cost=temp.path_cost+input.edges[x][i];
						//in_queueBFS.push(p);

						if (presentInQueue[p.input_id]==0)
						{
							in_queueBFS.push(p);
							presentInQueue[p.input_id]=1;
						}
						else if(presentInQueue[p.input_id]==1)
						{
							checkQueueBFS(p);
						}


					//	exp_queue.push(p);
					//	visited[p.input_id]=1;
						i++;
						// sorting
					}
				}
			out_queue.push(temp);
			if (visited[temp.input_id]==0)
				exp_queue.push(temp);
			visited[temp.input_id]=1;
			}

		}
	}
}

// DFS

node dfs()
{
	//cout << "inside algorithm bfs"<<"\n";
	//input.display();
	int node_number=1;
	int depth_number=0;
	//int *visited;
	visited = new int [input.number_of_participants];
	presentInQueue = new int [input.number_of_participants];
	//cout << "created visited"<<"\n";
	for (int i=0;i<input.number_of_participants;i++)
		visited[i]=0;
	//cout << "initialized visited"<<"\n";
	for (int i=0;i<input.number_of_participants;i++)
		presentInQueue[i]=0;

	node n,temp;
	string source=input.endpoints[0];
	string destination=input.endpoints[1];
	//cout << "Got source and destination"<<"\n";
	n.id=node_number;
	node_number++;
	n.state=source;
	n.input_id=getInputID(source);
	n.depth=depth_number;
	n.parent=-1;
	n.parent_name="";
	n.path_cost=0;
	//in_queueDFS.push(n);
	if (presentInQueue[n.input_id]==0)
	{
		in_queueDFS.push(n);
		presentInQueue[n.input_id]=1;
	}
	else if(presentInQueue[n.input_id]==1)
	{
		checkQueueDFS(n);
	}
	
	
	
	
	//exp_queue.push(n);
	//visited[n.input_id]=1;
	//cout << "inside algorithm bfs"<<"\n";
	while (true)
	{
		if (in_queueDFS.empty())
		{
			node ret;
			ret.state="";
			return ret;
		}
		else
		{
			temp = in_queueDFS.top();
			//temp = in_queueDFS.front();
			in_queueDFS.pop();
			//out_queue.push(temp);
			//exp_queue.push(temp);
			//visited[temp.input_id]=1;
		//	cout << "processing: " <<temp.id<<"--->" <<temp.state<<"-->"<<temp.parent<<"-->"<<temp.parent_name<<"\n";
			if (temp.state == destination)
			{
				exp_queue.push(temp);
				return temp;
			}
			else
			{
				//depth_number++;
				int x = getInputID(temp.state);
				for (int i=0;i<input.number_of_participants;)
				{
					
					if (input.edges[x][i]==0 || visited[i]==1)
						i++;
					else 
					{
						node p;
						p.id=node_number;
						node_number++;
						p.parent=temp.id;
						p.depth=temp.depth+1;
						p.state=input.participant_name[i].state;
						p.input_id=input.participant_name[i].input_id;
						p.parent_name=temp.state;
						p.path_cost=temp.path_cost+input.edges[x][i];
						//in_queueDFS.push(p);
						if (presentInQueue[p.input_id]==0)
						{
							in_queueDFS.push(p);
							presentInQueue[p.input_id]=1;
						}
						else if(presentInQueue[p.input_id]==1)
						{
							checkQueueDFS(p);
						}
					//	exp_queue.push(p);
					//	visited[p.input_id]=1;
						i++;
						// sorting
					}
				}
			out_queue.push(temp);
			if (visited[temp.input_id]==0)
				exp_queue.push(temp);
			visited[temp.input_id]=1;
			}

		}
	}
}

//UCS

node ucs()
{
	//cout << "inside algorithm bfs"<<"\n";
	//input.display();
	int node_number=1;
	int depth_number=0;
	//int *visited;
	visited = new int [input.number_of_participants];
	presentInQueue = new int [input.number_of_participants];
	//cout << "created visited"<<"\n";
	for (int i=0;i<input.number_of_participants;i++)
		visited[i]=0;
	//cout << "initialized visited"<<"\n";
	for (int i=0;i<input.number_of_participants;i++)
		presentInQueue[i]=0;
	node n,temp;
	string source=input.endpoints[0];
	string destination=input.endpoints[1];
	//cout << "Got source and destination"<<"\n";
	n.id=node_number;
	node_number++;
	n.state=source;
	n.input_id=getInputID(source);
	n.depth=depth_number;
	n.parent=-1;
	n.parent_name="";
	n.path_cost=0;
	//in_queueUCS.push(n);
	if (presentInQueue[n.input_id]==0)
	{
		in_queueUCS.push(n);
//		cout <<"pushed "<<n.state<<" with pathcost "<<n.path_cost;
		presentInQueue[n.input_id]=1;
	}
	else if(presentInQueue[n.input_id]==1)
	{
		checkQueueUCS(n);
	}
	
	//exp_queue.push(n);
	//visited[n.input_id]=1;
	//cout << "inside algorithm bfs"<<"\n";
	while (true)
	{
		if (in_queueUCS.empty())
		{
			node ret;
			ret.state="";
			return ret;
		}
		else
		{
			temp = in_queueUCS.top();
			//temp = in_queueUCS.front();
			in_queueUCS.pop();
			//out_queue.push(temp);
			//exp_queue.push(temp);
			//visited[temp.input_id]=1;
	//		cout << "processing: " <<temp.id<<"--->" <<temp.state<<"-->"<<temp.parent<<"-->"<<temp.parent_name<<"\n";
			if (temp.state == destination)
			{
				exp_queue.push(temp);
				return temp;
			}
			else
			{
				//depth_number++;
				int x = getInputID(temp.state);
				//cout <<"Got ID "<<x<<" for"<<temp.state<<"\n";
				for (int i=0;i<input.number_of_participants;)
				{
		//			cout << "**************"<<"\n";
		//			cout << "x: "<<x<<" i: "<<i;
		//			cout << "**************"<<"\n";
					if (input.edges[x][i]==0 || visited[i]==1)
						i++;
					else 
					{
						node p;
						p.id=node_number;
						node_number++;
						p.parent=temp.id;
						p.depth=temp.depth+1;
						p.state=input.participant_name[i].state;
						p.input_id=input.participant_name[i].input_id;
						p.parent_name=temp.state;
						p.path_cost=temp.path_cost+input.edges[x][i];
//						cout <<"****cost for: "<<p.state<<" : "<<p.path_cost<<" with parent "<<p.parent_name<<"****";
	//					cout <<"\n";
						//in_queueUCS.push(p);

						if (presentInQueue[p.input_id]==0)
						{
							in_queueUCS.push(p);
							presentInQueue[p.input_id]=1;
						}
						else if(presentInQueue[p.input_id]==1)
						{
							checkQueueUCS(p);
						}
					//	exp_queue.push(p);
					//	visited[p.input_id]=1;
						i++;
						// sorting
					}
				}
			out_queue.push(temp);
			if (visited[temp.input_id]==0)
				exp_queue.push(temp);
			visited[temp.input_id]=1;
			}

		}
	}
}

};





int main() {
   agent_input input;
	input.initialize();
	//input.display();
	algorithm algo_choice;
	//int status=0;
if (input.algo_type == 1)
	{
		algo_choice.input=input;
		node status = algo_choice.bfs();
		algo_choice.printResults(status);
	//	cout << "Exiting with final status "<<status;
		//cout << "Algo choice input";
		//algo_choice.input.display();
		
		/*status = algo_choice.getInputID("Andy");
		cout << "status:" <<status<<"\n";

		status = algo_choice.getInputID("Bill");
		cout << "status:" <<status<<"\n";

		status = algo_choice.getInputID("Claire");
		cout << "status:" <<status<<"\n";
		
		status = algo_choice.getInputID("Daniel");
		cout << "status:" <<status<<"\n";

		status = algo_choice.getInputID("Elaine");
		cout << "status:" <<status<<"\n";

		status = algo_choice.getInputID("Zoe");
		cout << "status:" <<status<<"\n";*/
	}
else if (input.algo_type == 2)
	{
		algo_choice.input=input;
		node status = algo_choice.dfs();
		algo_choice.printResults(status);
	}
else if (input.algo_type == 3)
	{
		algo_choice.input=input;
		node status = algo_choice.ucs();
		algo_choice.printResults(status);
	}
	/*else if (input.algo_type == 2)
		dfs(input);
	else if (input.algo_type == 1)
		ucs(input);*/
		return 0;

}

