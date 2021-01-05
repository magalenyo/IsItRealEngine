#pragma once
class Component
{
public:
	enum ComponentType {
		Material,
		Mesh,
		Transform
	};

	virtual void Enable() {};
	virtual void Update() {};
	virtual void Disable() {};

private:
	ComponentType type;
};

