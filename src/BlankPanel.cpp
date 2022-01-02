#include "JWModules.hpp"

struct BlankPanel : Module {BlankPanel() : Module(0, 0, 0, 0) {}};
struct BlankPanelSmallWidget : ModuleWidget { BlankPanelSmallWidget(BlankPanel *module); };
struct BlankPanelMediumWidget : ModuleWidget { BlankPanelMediumWidget(BlankPanel *module); };
struct BlankPanelLargeWidget : ModuleWidget { BlankPanelLargeWidget(BlankPanel *module); };

BlankPanelSmallWidget::BlankPanelSmallWidget(BlankPanel *module) : ModuleWidget(module) {
	box.size = Vec(15*3, 380);

	SVGPanel *panel = new SVGPanel();
	panel->box.size = box.size;
	panel->setBackground(SVG::load(assetPlugin(plugin, "res/BlankPanelSmall.svg")));
	addChild(panel);

	addChild(Widget::create<Screw_J>(Vec(16, 0)));
	addChild(Widget::create<Screw_W>(Vec(16, 365)));
}

BlankPanelMediumWidget::BlankPanelMediumWidget(BlankPanel *module) : ModuleWidget(module) {
	box.size = Vec(15*6, 380);

	SVGPanel *panel = new SVGPanel();
	panel->box.size = box.size;
	panel->setBackground(SVG::load(assetPlugin(plugin, "res/BlankPanelMedium.svg")));
	addChild(panel);

	addChild(Widget::create<Screw_J>(Vec(16, 0)));
	addChild(Widget::create<Screw_J>(Vec(16, 365)));
	addChild(Widget::create<Screw_W>(Vec(box.size.x-29, 0)));
	addChild(Widget::create<Screw_W>(Vec(box.size.x-29, 365)));
}

BlankPanelLargeWidget::BlankPanelLargeWidget(BlankPanel *module) : ModuleWidget(module) {
	box.size = Vec(15*12, 380);

	SVGPanel *panel = new SVGPanel();
	panel->box.size = box.size;
	panel->setBackground(SVG::load(assetPlugin(plugin, "res/BlankPanelLarge.svg")));
	addChild(panel);

	addChild(Widget::create<Screw_J>(Vec(16, 0)));
	addChild(Widget::create<Screw_J>(Vec(16, 365)));
	addChild(Widget::create<Screw_W>(Vec(box.size.x-29, 0)));
	addChild(Widget::create<Screw_W>(Vec(box.size.x-29, 365)));
}

Model *modelBlankPanelSmall = Model::create<BlankPanel, BlankPanelSmallWidget>("JW-Modules", "BlankPanel_SM", "BlankPanel (Small)",  BLANK_TAG);
Model *modelBlankPanelMedium = Model::create<BlankPanel, BlankPanelMediumWidget>("JW-Modules", "BlankPanel_MD", "BlankPanel (Medium)",  BLANK_TAG);
Model *modelBlankPanelLarge = Model::create<BlankPanel, BlankPanelLargeWidget>("JW-Modules", "BlankPanel_LG", "BlankPanel (Large)",  BLANK_TAG);
