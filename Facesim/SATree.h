#ifndef SATREE_H
#define SATREE_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <functional>

#define INFINITE 10000000

using namespace std;

typedef float Dis;
typedef int Dimension;
typedef int ID;

template <typename kPoint>
class SATree{
	private:
		
		class Node;

	public:

		typedef Dis (*FunDis)(kPoint,kPoint);
		typedef vector<Node*> SetOfNodes;
		typedef vector<kPoint> SetOfKPoints;
		typedef kPoint Query;
		typedef int Quantity;
		typedef function<bool (Node *,Node *)> CompFun;

		SATree(){
			distance = nullptr;
			root = nullptr;			
			idActual = 0;
			dim = 0;
		}
		SATree(FunDis dis, SetOfKPoints S){
			idActual = 0;
			dim = 0;
			distance = dis;
			buildTree(S);
		}
		void buildTree(SetOfKPoints S);
		void print(string file);
		SetOfKPoints rangeSearch(Query query, Dis radius);
		void printSetOfKPoints(SetOfKPoints set);
		void printKPoint(kPoint point);
		SetOfKPoints NNSearch(Query query, Quantity k);
	private:

		class Node{
			public:
				Node(){
					id = -1;
				}
				Node(kPoint val, ID id){
					this->val = val;
					this->id = id;
					init();
				}
				void init(){
					neighbors.clear();
					coverRadio = 0;
					disToNode = 0;
					minDis = 0;
					maxDis = 0;
				};
				kPoint getVal(){return val;}
				void setDisToNode(Dis dis){disToNode = dis;}
				void setTBound(Dis dis){disToNode = dis;}
				void setCoverRadio(Dis radio){coverRadio = radio;}
				void setMinDis(Dis min){minDis = min;}
				void setMaxDis(Dis max){maxDis = max;}
				void addNeighbors(Node * node){neighbors.push_back(node);}
				void addSubTree(Node * node){subTrees.push_back(node);}
				void resetSubTrees(){subTrees.clear();}
				Dis getDisToNode(){return disToNode;}
				Dis getTBound(){return disToNode;}
				Dis getCoverRadio(){return coverRadio;}
				Dis getMinDis(){return minDis;}
				Dis getMaxDis(){return maxDis;}
				SetOfNodes getNeighbors(){return neighbors;}
				SetOfNodes & getSubTrees(){return subTrees;}
				ID getId(){return id;}
			private:
				kPoint val;
				ID id;
				SetOfNodes neighbors;
				SetOfNodes subTrees;
				Dis coverRadio;
				Dis disToNode;
				Dis minDis;
				Dis maxDis;
				
		};
		Dimension dim;
		FunDis distance;
		Node * root;
		ID idActual;

		void _buildTree(Node * actual, SetOfNodes & set);
		void _rangeSearch(Node * actual, Query query, Dis radius, Dis mind, SetOfKPoints & resPoints);
		Dis _sortByDistance(Node * actual, SetOfNodes & set);
		void _addHeap(SetOfNodes & heap, Node * node, CompFun fun);
		Node * _popHeap(SetOfNodes & heap, CompFun fun);
		SetOfNodes _NNSearch(Query query, Quantity k);
};

template <typename kPoint>
typename SATree<kPoint>::SetOfKPoints SATree<kPoint>::NNSearch(Query query, Quantity k){
	SetOfNodes nodes = _NNSearch(query,k);
	sort(nodes.begin(), nodes.end(),[](Node * a, Node * b){
		return a->getMaxDis() < b->getMaxDis();
	});
	SetOfKPoints res;
	for(Node * n : nodes){
		res.push_back(n->getVal());
	}
	return res;
}

template <typename kPoint>
void SATree<kPoint>::_addHeap(SetOfNodes & heap, Node * node, CompFun fun){
	heap.push_back(node);
	push_heap(heap.begin(), heap.end(), fun);
}

template <typename kPoint>
typename SATree<kPoint>::Node * SATree<kPoint>::_popHeap(SetOfNodes & heap, CompFun fun){
	Node * temp = heap.front();
	pop_heap(heap.begin(), heap.end(), fun);
	heap.pop_back();
	return temp;
}

template <typename kPoint>
typename SATree<kPoint>::SetOfNodes SATree<kPoint>::_NNSearch(Query query, Quantity k){
	SetOfNodes resPoints;
	SetOfNodes heap;
	CompFun minTBound = [](Node * a, Node * b){
		return a->getDisToNode() > b->getDisToNode();
	};
	CompFun maxDis = [](Node * a, Node * b){
		return a->getMaxDis() < b->getMaxDis();
	};
	Dis radius = root->getCoverRadio() + INFINITE;
	Dis rootDis = distance(query,root->getVal());
	root->setTBound(max((Dis) 0, rootDis - root->getCoverRadio()));
	root->setMinDis(rootDis);
	_addHeap(heap,root,minTBound);
	while(!heap.empty()){
		Node * actual = _popHeap(heap,minTBound);
		Dis tBound = actual->getTBound();
		Dis minDis = actual->getMinDis();
		if(tBound > radius) return resPoints;
		actual->setMaxDis(distance(query,actual->getVal()));
		_addHeap(resPoints,actual,maxDis);
		if(resPoints.size() == k + 1) _popHeap(resPoints,maxDis);
		if(resPoints.size() == k) radius = resPoints.front()->getMaxDis();
		for(Node * c : actual->getNeighbors()){
			Dis temp = distance(c->getVal(),query);
			if(minDis > temp) minDis = temp;
		}
		for(Node * v : actual->getNeighbors()){
			Dis tt = minDis/2.0;
			Dis temp = max(tBound, tt);
			v->setTBound(max(temp,distance(query,v->getVal()) - v->getCoverRadio()));
			v->setMinDis(minDis);
			_addHeap(heap,v,minTBound);
		}
	}
	return resPoints;
}

template <typename kPoint>
void SATree<kPoint>::printKPoint(kPoint point){
	for(int i = 0; i < point.size(); i++){
		cout<<point[i];
		if(i != point.size() - 1) cout<<",";
		else cout<<endl;
	}
}

template <typename kPoint>
void SATree<kPoint>::printSetOfKPoints(SetOfKPoints set){
	for(kPoint point : set){
		printKPoint(point);
	}
}

template <typename kPoint>
typename SATree<kPoint>::SetOfKPoints SATree<kPoint>::rangeSearch(Query query, Dis radius){
	SetOfKPoints resPoint;	
	_rangeSearch(root, query, radius, distance(root->getVal(),query), resPoint);
	return resPoint;
}

template <typename kPoint>
void SATree<kPoint>::_rangeSearch(Node * actual, Query query, Dis radius, Dis mind, SetOfKPoints & resPoints){
	if(distance(actual->getVal(), query) <= actual->getCoverRadio() + radius){
		if(distance(actual->getVal(), query) <= radius) resPoints.push_back(actual->getVal());
		for(Node * c : actual->getNeighbors()){
			Dis tempDis = distance(query, c->getVal());
			if(tempDis < mind) mind = tempDis;
		}
		for(Node * b : actual->getNeighbors()){
			if(distance(b->getVal(), query) <= mind + 2 * radius){
				_rangeSearch(b, query, radius, mind, resPoints);
			}
		}
	}
}

template <typename kPoint>
void SATree<kPoint>::buildTree(SetOfKPoints setKPoints){
	srand(time(NULL));
	//int randNumber = rand() % setKPoints.size();
	int randNumber = 3;
	SetOfNodes set;
	root = new Node(setKPoints[randNumber], idActual);
	idActual++;
	setKPoints.erase(setKPoints.begin() + randNumber);
	for(kPoint point : setKPoints){
		set.push_back(new Node(point,idActual));
		idActual++;
	}

	_buildTree(root,set);
}

//template <typename kPoint>
//bool _funSortByDistance(SATree<kPoint>::Node * a, SATree<kPoint>::Node * b){
//	return a->getDisToNode() < b->getDisToNode();
//}

template <typename kPoint>
Dis SATree<kPoint>::_sortByDistance(Node * actual, SetOfNodes & set){
	for(Node * n : set){
		n->setDisToNode(distance(n->getVal(),actual->getVal()));
	}
	sort(set.begin(),set.end(),[](Node * a, Node * b){
		return a->getDisToNode() < b->getDisToNode();
	});
}

template <typename kPoint>
void SATree<kPoint>::_buildTree(Node * actual, SetOfNodes & set){
	if(set.size() == 0 or actual == nullptr) return;
	actual->init();
	_sortByDistance(actual, set);
	
	for(auto iter = set.begin(); iter != set.end(); ++iter){
		Node * v = *iter;
		Dis tt = max(actual->getCoverRadio(),v->getDisToNode());
		actual->setCoverRadio(tt);
		if(actual->getNeighbors().size() == 0){
			actual->addNeighbors(v);
			set.erase(iter);
			--iter;
		}
		else{
			bool flag = true;
			for(Node * b : actual->getNeighbors()){
				if(!(v->getDisToNode() < distance(v->getVal(),b->getVal()))){
					flag = false;
					break;
				}
			}	
			if(flag){
				actual->addNeighbors(v);
				set.erase(iter);
				--iter;
			} 
		}
	}

	//for(Nodo * b : actual->getNeighbors()){
	//	b->resetSubTrees();
	//}
	for(Node * v : set){
		Node * temp = nullptr;
		for(Node * c : actual->getNeighbors()){
			c->setDisToNode(distance(v->getVal(), c->getVal()));
			if(temp == nullptr or temp->getDisToNode() > c->getDisToNode()){
				temp = c;
			}
		}
		temp->addSubTree(v);
	}
	for(Node * b : actual->getNeighbors()){
		_buildTree(b,b->getSubTrees());
	}
}	

template <typename kPoint>
void SATree<kPoint>::print(string file){
	string fi = file + ".dot";
	ofstream archivo(fi.c_str());
	archivo<<"graph{"<<endl;
	SetOfNodes q;
	q.push_back(root);
	while(q.size() != 0){
		Node * actual = q.front();
		q.erase(q.begin());
		string n = "Node" + to_string(actual->getId());
		string n1 = n;
		n+="[label = \"";

		for(int i = 0; i < actual->getVal().size(); i++){
			n+= to_string((int)actual->getVal()[i]);
			n+=",";
		}

		n.pop_back();
		n+="\"];";

		archivo<<n<<endl;

		for(Node * v : actual->getNeighbors()){
			q.push_back(v);
			string n2 = "Node" + to_string(v->getId());
			string dir = n1 + " -- " +  n2;
			archivo<<dir<<";"<<endl;
		}
	}
	archivo<<"}";
	archivo.close();
	string command = "dot -Tpng " + fi + " -o " + file + ".png";
	system(command.c_str());
}


#endif
