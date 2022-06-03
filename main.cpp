// #define OLC_PGE_APPLICATION

// #include<iostream>
// #include "olcPixelGameEngine.h"

// using namespace std;
// int main(){

// cout<<"hello world.\n";

//     return 0;
// }

#include "olcPixelGameEngine.h"


struct sShape;

struct sNode{
    sShape *parent;
    olc::vf2d pos;
};




struct sShape{
    std::vector<sNode> vecNodes;
    uint32_t nMaxNodes = 0;
    olc::Pixel col = olc::GREEN;
// All shapes share word to screen transformation
	// coefficients, so share them staically
	static float fWorldScale;
	static olc::vf2d vWorldOffset;

	// Convert coordinates from World Space --> Screen Space
	void WorldToScreen(const olc::vf2d &v, int &nScreenX, int &nScreenY)
	{
		nScreenX = (int)((v.x - vWorldOffset.x) * fWorldScale);
		nScreenY = (int)((v.y - vWorldOffset.y) * fWorldScale);
	}

	// This is a PURE function, which makes this class abstract. A sub-class
	// of this class must provide an implementation of this function by
	// overriding it
	virtual void DrawYourself(olc::PixelGameEngine *pge) = 0;

	// Shapes are defined by nodes, the shape is responsible
	// for issuing nodes that get placed by the user. The shape may
	// change depending on how many nodes have been placed. Once the
	// maximum number of nodes for a shape have been placed, it returns
	// nullptr
	sNode* GetNextNode(const olc::vf2d &p)
	{
		if (vecNodes.size() == nMaxNodes)
			return nullptr; // Shape is complete so no new nodes to be issued

		// else create new node and add to shapes node vector
		sNode n;
		n.parent = this;
		n.pos = p;
		vecNodes.push_back(n);

		// Beware! - This normally is bad! But see sub classes
		return &vecNodes[vecNodes.size() - 1];
	}

	// Test to see if supplied coordinate exists at same location
	// as any of the nodes for this shape. Return a pointer to that
	// node if it does
	sNode* HitNode(olc::vf2d &p)
	{
		for (auto &n : vecNodes)
		{
			if ((p - n.pos).mag() < 0.01f)
				return &n;
		}

		return nullptr;
	}

	// Draw all of the nodes that define this shape so far
	void DrawNodes(olc::PixelGameEngine *pge)
	{
		for (auto &n : vecNodes)
		{
			int sx, sy;
			WorldToScreen(n.pos, sx, sy);
			pge->FillCircle(sx, sy, 2, olc::RED);
		}
	}

};







class DesignTool : public olc::PixelGameEngine{
    private:
        olc::vf2d vOffset = { 0.0f, 0.0f};
        olc::vf2d vStartPan = { 0.0f, 0.0f };
        float fScale = 10.0f;
        float fGrid = 1.0f;

        void WorldToScreen(const olc::vf2d &v, int& nScreenX, int& nScreenY){
            nScreenX = (int)((v.x - vOffset.x) * fScale);
            nScreenY = (int)((v.y - vOffset.y) * fScale);
        }

        void ScreenToWorld(int nScreenX, int nScreenY, olc::vf2d &v){
            v.x = (float)(nScreenX) / fScale + vOffset.x;
            v.y = (float)(nScreenY) /fScale + vOffset.y;
        }
   
    public:
        DesignTool(){//constructor
            sAppName = "Polymorphism";
        }

        bool OnUserCreate() override{//overrides mean that the compiler will throw an error if the signature of the function changes from the base class function
            vOffset = {(float)(-ScreenWidth() / 2) /fScale, (float) (-ScreenHeight()/2)};

            return true;
        }

        bool OnUserUpdate(float fElapsedTime) override{
            // Get mouse location this frame
		olc::vf2d vMouse = { (float)GetMouseX(), (float)GetMouseY() };


		// Handle Pan & Zoom
		if (GetMouse(2).bPressed)
		{
			vStartPan = vMouse;
		}

		if (GetMouse(2).bHeld)
		{
			vOffset -= (vMouse - vStartPan) / fScale;
			vStartPan = vMouse;
		}

		olc::vf2d vMouseBeforeZoom;
		ScreenToWorld((int)vMouse.x, (int)vMouse.y, vMouseBeforeZoom);

		if (GetKey(olc::Key::Q).bHeld || GetMouseWheel() > 0)
		{
			fScale *= 1.1f;
		}

		if (GetKey(olc::Key::A).bHeld || GetMouseWheel() < 0)
		{
			fScale *= 0.9f;
		}

		olc::vf2d vMouseAfterZoom;
		ScreenToWorld((int)vMouse.x, (int)vMouse.y, vMouseAfterZoom);
		vOffset += (vMouseBeforeZoom - vMouseAfterZoom);


		// Snap mouse cursor to nearest grid interval
		vCursor.x = floorf((vMouseAfterZoom.x + 0.5f) * fGrid);
		vCursor.y = floorf((vMouseAfterZoom.y + 0.5f) * fGrid);


		if (GetKey(olc::Key::L).bPressed)
		{
			tempShape = new sLine();

			// Place first node at location of keypress
			selectedNode = tempShape->GetNextNode(vCursor);

			// Get Second node
			selectedNode = tempShape->GetNextNode(vCursor);
		}


		if (GetKey(olc::Key::B).bPressed)
		{
			tempShape = new sBox();

			// Place first node at location of keypress
			selectedNode = tempShape->GetNextNode(vCursor);

			// Get Second node
			selectedNode = tempShape->GetNextNode(vCursor);
		}

		if (GetKey(olc::Key::C).bPressed)
		{
			// Create new shape as a temporary
			tempShape = new sCircle();

			// Place first node at location of keypress
			selectedNode = tempShape->GetNextNode(vCursor);

			// Get Second node
			selectedNode = tempShape->GetNextNode(vCursor);
		}

		if (GetKey(olc::Key::S).bPressed)
		{
			// Create new shape as a temporary
			tempShape = new sCurve();

			// Place first node at location of keypress
			selectedNode = tempShape->GetNextNode(vCursor);

			// Get Second node
			selectedNode = tempShape->GetNextNode(vCursor);
		}

		// Search for any node that exists under the cursor, if one
		// is found then select it
		if (GetKey(olc::Key::M).bPressed)
		{
			selectedNode = nullptr;
			for (auto &shape : listShapes)
			{
				selectedNode = shape->HitNode(vCursor);
				if (selectedNode != nullptr)
					break;
			}
		}


		// If a node is selected, make it follow the mouse cursor
		// by updating its position
		if (selectedNode != nullptr)
		{
			selectedNode->pos = vCursor;
		}


		// As the user left clicks to place nodes, the shape can grow
		// until it requires no more nodes, at which point it is completed
		// and added to the list of completed shapes.
		if (GetMouse(0).bReleased)
		{
			if (tempShape != nullptr)
			{
				selectedNode = tempShape->GetNextNode(vCursor);
				if (selectedNode == nullptr)
				{
					tempShape->col = olc::WHITE;
					listShapes.push_back(tempShape);
					tempShape = nullptr; // Thanks @howlevergreen /Disord
				}
				
			}
			else
			{
				selectedNode = nullptr;
			}
		}



		// Clear Screen
		Clear(olc::VERY_DARK_BLUE);

		int sx, sy;
		int ex, ey;

		// Get visible world
		olc::vf2d vWorldTopLeft, vWorldBottomRight;
		ScreenToWorld(0, 0, vWorldTopLeft);
		ScreenToWorld(ScreenWidth(), ScreenHeight(), vWorldBottomRight);

		// Get values just beyond screen boundaries
		vWorldTopLeft.x = floor(vWorldTopLeft.x);
		vWorldTopLeft.y = floor(vWorldTopLeft.y);
		vWorldBottomRight.x = ceil(vWorldBottomRight.x);
		vWorldBottomRight.y = ceil(vWorldBottomRight.y);

		// Draw Grid dots
		for (float x = vWorldTopLeft.x; x < vWorldBottomRight.x; x += fGrid)
		{
			for (float y = vWorldTopLeft.y; y < vWorldBottomRight.y; y += fGrid)
			{
				WorldToScreen({ x, y }, sx, sy);
				Draw(sx, sy, olc::BLUE);
			}
		}

		// Draw World Axis
		WorldToScreen({ 0,vWorldTopLeft.y }, sx, sy);
		WorldToScreen({ 0,vWorldBottomRight.y }, ex, ey);
		DrawLine(sx, sy, ex, ey, olc::GREY, 0xF0F0F0F0);
		WorldToScreen({ vWorldTopLeft.x,0 }, sx, sy);
		WorldToScreen({ vWorldBottomRight.x,0 }, ex, ey);
		DrawLine(sx, sy, ex, ey, olc::GREY, 0xF0F0F0F0);

		// Update shape translation coefficients
		sShape::fWorldScale = fScale;
		sShape::vWorldOffset = vOffset;

		// Draw All Existing Shapes
		for (auto &shape : listShapes)
		{
			shape->DrawYourself(this);
			shape->DrawNodes(this);
		}

		// Draw shape currently being defined
		if (tempShape != nullptr)
		{
			tempShape->DrawYourself(this);
			tempShape->DrawNodes(this);
		}

		// Draw "Snapped" Cursor
		WorldToScreen(vCursor, sx, sy);
		DrawCircle(sx, sy, 3, olc::YELLOW);

		// Draw Cursor Position
		DrawString(10, 10, "X=" + std::to_string(vCursor.x) + ", Y=" + std::to_string(vCursor.x), olc::YELLOW, 2);
		return true;
        }
};



int main(){

    DesignTool demo;
    if(demo.Construct(1600, 960, 1, 1)){//pge function since it is the parent class of DesignTool
        demo.Start();
    }
    return 0;
}