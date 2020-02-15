#ifndef __GAMEBEHAVIOUR_H__
#define __GAMEBEHAVIOUR_H__


//Class GameBehaviour: Start and Update
class GameBehaviour {


public:
	virtual ~GameBehaviour(){}
	//Start
	virtual void Start() = 0;
	//Update
	virtual void Update(const float dt) = 0;

protected:
	GameBehaviour() {};
};

#endif