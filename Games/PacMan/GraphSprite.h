#pragma once

#include "GraphNode.h"
#include "Sprite.h"
class GraphSprite : public Sprite
{

	public:
		GraphSprite();
		~GraphSprite();

		void update();

		GraphNode* getCurrentNode()							{   return currentNode;     }
		GraphNode* getDestinationNode()						{   return destinationNode; }

		void		setCurrentNode(GraphNode* node);
		void		setDestinationNode(GraphNode* node);
		void        reverseDirection();


	private:
		GraphNode* currentNode     = nullptr;
		GraphNode* destinationNode = nullptr;
		GraphNode* previousNode	   = nullptr;

		GraphNode* findNextNode();

		bool active = true;

		
};

