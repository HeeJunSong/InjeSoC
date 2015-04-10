#include "stdafx.h"
#include "EdgeMaster.h"
#include "SoC ProjectView.h"

namespace EdgeMaster{
	// EdgeObject
	EdgeObject::EdgeObject(){
		DOT_HeadPtr = new DOT();
		DOT_TailPtr = DOT_HeadPtr;
		DOT_TailPtr->nextPtr = NULL;
		edge_len = 0;
	}
	EdgeObject::~EdgeObject(){
		while(DOT_HeadPtr->nextPtr != NULL)
		{
			DOT * tempPtr = DOT_HeadPtr->nextPtr;
			DOT_HeadPtr->nextPtr = tempPtr->nextPtr;
			if(tempPtr->nextPtr != NULL) delete tempPtr;
		}
		//delete []DOT_HeadPtr;
		/*unsigned int id;
		_beginthreadex(NULL,0, ReleasePool_OBJECT,(void*)this,0,&id);*/
	}
	void EdgeObject::AddVertex(long x, long y)
	{
		DOT_TailPtr->nextPtr = new DOT(x,y);
		DOT_TailPtr = DOT_TailPtr->nextPtr;
		DOT_TailPtr->nextPtr = NULL;
		edge_len++;
	}
	
	/*void EdgeObject::DrawEdge(int * RGB, COLORREF color)
	{
		int R = GetRValue(color),
			G = GetGValue(color),
			B = GetBValue(color), i=0;
		if(edge != NULL) DOT_NowPtr = edge;
		static const RECT size = CSoCProjectView::GetIMGSize();
		long width = size.right;
		while(DOT_NowPtr != DOT_TailPtr)
		{
			RGB[width * DOT_NowPtr->y + DOT_NowPtr->x] = R;
			RGB[width * DOT_NowPtr->y + DOT_NowPtr->x + 1] = G;
			RGB[width * DOT_NowPtr->y + DOT_NowPtr->x + 2] = B;
			DOT_NowPtr = DOT_NowPtr->nextPtr;
		}
	}*/

	// EdgeObjectList
	EdgeObjectList::EdgeObjectList(){
		HeadItem = new EdgeObject[1];
		TailItem = HeadItem;
		TailItem->nextPtr = HeadItem;
		HeadItem->prePtr = TailItem;
		COUNT = 0;
	}
	/*EdgeObjectList::~EdgeObjectList(){
		while() delete headPtr->nextPtr;
		delete headPtr;
	}*/
	void EdgeObjectList::AddItem(EdgeObject *obj)
	{
		TailItem->nextPtr = obj;
		TailItem->nextPtr->prePtr = TailItem;
		TailItem = TailItem->nextPtr;
		TailItem->nextPtr = HeadItem;
		HeadItem->prePtr = TailItem;
		COUNT++;
	}
	EdgeObject EdgeObjectList::GetItem(int index)
	{
		if(index >= COUNT) throw("Error : Out of range!! - (EdgeObjectList::GetItem)");
		return HeadItem[index+1];
	}
	void EdgeObjectList::Refresh()
	{
		/*unsigned int id;
		_beginthreadex(NULL,0, ReleasePool_LIST,(void*)this,0,&id);*/
		while(HeadItem != HeadItem->nextPtr)
		{
			EdgeObject * temp = HeadItem;
			HeadItem = HeadItem->nextPtr;
			TailItem->nextPtr = HeadItem;
			/*while(NowItem->DOT_HeadPtr != NowItem->DOT_HeadPtr->nextPtr)
			{
				NowItem->DOT_NowPtr = NowItem->DOT_HeadPtr;
				NowItem->DOT_HeadPtr = NowItem->DOT_HeadPtr->nextPtr;
				delete NowItem->DOT_NowPtr;
			} */
			if(COUNT > 0) delete [] temp;
			else break;
			COUNT--;
		}
		COUNT = 0;
	}
}