#ifndef QUADTREENODE_H
#define QUADTREENODE_H

#include "QuadTreeOccupant.h"
#include <vector>
#include <unordered_set>

const unsigned int maximumOccupants = 6;
const unsigned int minimumOccupants = 1;

const unsigned int maxLevels = 20;

class QuadTree;

class QuadTreeNode
{
private:
	AABB region;
	Vec2f center;

	QuadTreeNode* pParentNode;
	QuadTree* pQuadTree;

	std::unordered_set<QuadTreeOccupant*> occupants;

	QuadTreeNode* children[2][2];
	bool hasChildren;

	unsigned int numOccupants;

	unsigned int level;

	void Partition();

	void DestroyChildren();

	void Merge();
	void GetOccupants(std::unordered_set<QuadTreeOccupant*> &upperOccupants, QuadTreeNode* newNode);
	Point2i GetPossibleOccupantPos(QuadTreeOccupant* pOc);

public:
	QuadTreeNode(const AABB &newRegion, unsigned int numLevels, QuadTreeNode* pParent = NULL, QuadTree* pContainer = NULL);
	~QuadTreeNode();

	void AddOccupant(QuadTreeOccupant* pOc);
	void Query(const AABB &queryRegion, std::vector<QuadTreeOccupant*> &queryResult);

	friend class QuadTreeNode;
	friend class QuadTreeOccupant;
	friend class QuadTree;
};

#endif
