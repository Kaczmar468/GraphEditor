#include "algorithms.h"
#include <vector>
#include <string>
#include <set>
#include <unordered_map>
#include <queue>
#include <stack>
#include <iostream>

Algo::Algo(){

}

void Algo::set_graph(std::vector<int> v, std::set <std::pair<int,int> > paths){
	conv_m.clear();
	graph.clear();
	graph.resize(v.size());
	for(int i=1;i<(int)v.size();i++)
		conv_m[v[i]]=i;
	source=conv_m[v[0]];
	for(auto u:paths){
		graph[conv_m[u.first]].push_back(conv_m[u.second]);
		graph[conv_m[u.second]].push_back(conv_m[u.first]);
	}
	conv_v=v;
}

std::set <std::pair<int,int> > Algo::BFS(){
	std::set <std::pair<int,int> > ans;
	std::queue <int> q;
	int size=conv_v.size();
	bool visited[size+2];
	for(int i=1;i<=size+1;i++)
		visited[i]=0;
	q.push(source);
	visited[source]=1;
	while(!q.empty()){
		int cur=q.front();
		q.pop();
		for(auto u:graph[cur])
			if(!visited[u]){
				q.push(u);
				visited[u]=1;
				ans.insert({std::min(conv_v[u],conv_v[cur]),std::max(conv_v[u],conv_v[cur])});
			}
	}
	return ans;
}


std::set <std::pair<int,int> > Algo::DFS(){
	std::set <std::pair<int,int> > ans;
	std::stack <int> q;
	int size=conv_v.size();
	bool visited[size+2];
	for(int i=1;i<=size+1;i++)
		visited[i]=0;
	q.push(source);
	visited[source]=1;
	while(!q.empty()){
		int cur=q.top();
		q.pop();
		for(auto u:graph[cur])
			if(!visited[u]){
				q.push(u);
				visited[u]=1;
				ans.insert({std::min(conv_v[u],conv_v[cur]),std::max(conv_v[u],conv_v[cur])});
			}
	}
	return ans;
}

int Algo::nocc(){
	int ans=0;
	int size=conv_v.size();
	bool visited[size+2];
	std::stack <int> q;
	for(int i=1;i<=size+1;i++)
		visited[i]=0;
	for(int i=1;i<size;i++){
		if(!visited[i]){
			q.push(i);
			visited[i]=1;
			while(!q.empty()){			
				int cur=q.top();
				q.pop();
				for(auto u:graph[cur])
					if(!visited[u]){
						q.push(u);
						visited[u]=1;
					}
			}
			ans++;
		}
	}
	return ans;
}