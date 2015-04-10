/*
작성자 : 송희준
최종 작성일 : 14.05.19
소속 : C&C Lab. - Inje University
문의 : spidermine2@gmail.com

Copyright(C)2014 by Hee-Jun Song.
All rights reserved.
*/
#include "stdafx.h"

namespace EdgeMaster{
	typedef struct DOT{
		long x, y;
		DOT*nextPtr;
		DOT(){
			this->x = 0;
			this->y = 0;
		}
		DOT(long x, long y)
		{
			this->x = x;
			this->y = y;
		}
	}DOT;
	class EdgeObject{
	public:
		EdgeObject * nextPtr;
		EdgeObject * prePtr;
		DOT * DOT_HeadPtr;
		DOT * DOT_TailPtr;
		RECT rect; // 객체가 담긴 사각형 그릇
		POINT gravity;
		int edge_len;
		EdgeObject();
		~EdgeObject();
		void AddVertex(long x, long y);
		//unsigned int _stdcall ReleasePool(void * relDOT);
		//void DrawEdge(int * RGB, COLORREF color);
		friend class EdgeObjectList;
	};
	class EdgeObjectList{
	public:
		int COUNT;
		EdgeObject*HeadItem;
		EdgeObject*TailItem;
		void AddItem(EdgeObject *obj);
		EdgeObject GetItem(int index);
		void Refresh();
		EdgeObjectList();
		//~EdgeObjectList();
	};
	unsigned int _stdcall ReleasePool_OBJECT(void *);
	unsigned int _stdcall ReleasePool_LIST(void *);
};