#include "JWModules.hpp"
#include "dsp/digital.hpp"

struct WavHead : Module {
	enum ParamIds {
		NUM_PARAMS
	};
	enum InputIds {
		VOLT_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};
	
	bool invert = true;
	bool neg5ToPos5 = false;
	WavHead() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}
	
	json_t *toJson() override {
		json_t *rootJ = json_object();
		json_object_set_new(rootJ, "invert", json_boolean(invert));
		json_object_set_new(rootJ, "neg5ToPos5", json_boolean(neg5ToPos5));
		return rootJ;
	}

	void fromJson(json_t *rootJ) override {
		json_t *invertJ = json_object_get(rootJ, "invert");
		if (invertJ){ invert = json_is_true(invertJ); }

		json_t *neg5ToPos5J = json_object_get(rootJ, "neg5ToPos5");
		if (neg5ToPos5J){ neg5ToPos5 = json_is_true(neg5ToPos5J); }
	}

};

struct WavHeadWidget : ModuleWidget {
	WavHeadWidget(WavHead *module);
	void step() override;
	Widget* widgetToMove;
	Menu *createContextMenu() override;
};

void WavHeadWidget::step() {
	WavHead *wavHead = dynamic_cast<WavHead*>(module);
	float minVolts = wavHead->neg5ToPos5 ? -5 : 0;
	float maxVolts = minVolts + 10;
	float clamped = clampfjw(module->inputs[WavHead::VOLT_INPUT].value, minVolts, maxVolts);
	float minY = wavHead->invert ? 250 : 15;
	float maxY = wavHead->invert ? 15 : 250;
	float y = rescalefjw(clamped, minVolts, maxVolts, minY, maxY);;
	if (widgetToMove->box.pos.y != y)
	{
		widgetToMove->box.pos.y = y;
		widgetToMove->dirty = true;
	}
};

WavHeadWidget::WavHeadWidget(WavHead *module) : ModuleWidget(module) {
	box.size = Vec(RACK_GRID_WIDTH*4, RACK_GRID_HEIGHT);

	LightPanel *panel = new LightPanel();
	panel->box.size = box.size;
	addChild(panel);

	widgetToMove = Widget::create<WavHeadLogo>(Vec(5, 250));
	addChild(widgetToMove);
	addChild(Widget::create<Screw_J>(Vec(16, 1)));
	addChild(Widget::create<Screw_J>(Vec(16, 365)));
	addChild(Widget::create<Screw_W>(Vec(box.size.x-29, 1)));
	addChild(Widget::create<Screw_W>(Vec(box.size.x-29, 365)));

	addInput(Port::create<PJ301MPort>(Vec(18, 330), Port::INPUT, module, WavHead::VOLT_INPUT));
}

Menu *WavHeadWidget::createContextMenu() {
	struct InvertMenuItem : MenuItem {
		WavHead *wavHead;
		void onAction(EventAction &e) override {
			wavHead->invert = !wavHead->invert;
		}
		void step() override {
			rightText = wavHead->invert ? "✔" : "";
		}
	};

	struct Neg5MenuItem : MenuItem {
		WavHead *wavHead;
		void onAction(EventAction &e) override {
			wavHead->neg5ToPos5 = !wavHead->neg5ToPos5;
		}
		void step() override {
			rightText = wavHead->neg5ToPos5 ? "✔" : "";
		}
	};


	Menu *menu = ModuleWidget::createContextMenu();
	WavHead *wavHead = dynamic_cast<WavHead*>(module);

	MenuLabel *spacerLabel = new MenuLabel();
	menu->addChild(spacerLabel);

	InvertMenuItem *invertMenuItem = new InvertMenuItem();
	invertMenuItem->text = "Invert";
	invertMenuItem->wavHead = wavHead;
	menu->addChild(invertMenuItem);

	Neg5MenuItem *neg5MenuItem = new Neg5MenuItem();
	neg5MenuItem->text = "-5 to +5";
	neg5MenuItem->wavHead = wavHead;
	menu->addChild(neg5MenuItem);

	return menu;
}

Model *modelWavHead = Model::create<WavHead, WavHeadWidget>("JW-Modules", "WavHead", "Wav Head", VISUAL_TAG);