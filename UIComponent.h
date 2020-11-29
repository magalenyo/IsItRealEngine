#pragma once
class UIComponent {
public:
	UIComponent() {};
	virtual ~UIComponent() {};

	virtual void Draw() {};
};