#ifndef __GAMEBEHAVIOUR_H__
#define __GAMEBEHAVIOUR_H__



class GameBehaviour {


public:
	virtual ~GameBehaviour(){}

	virtual void Start() = 0;
	virtual void Update(const float dt) = 0;

protected:
	GameBehaviour() {};
};

#endif