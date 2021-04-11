#include "ofApp.h"	

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(39);
	ofNoFill();

	ofColor color;
	vector<int> hex_list = { 0x1f005c, 0x5b0060, 0x870160, 0xac255e, 0xca485c, 0xe16b5c, 0xf39060, 0xffb56b };
	for (auto hex : hex_list) {

		color.setHex(hex);
		this->base_color_list.push_back(color);
	}

	this->font.loadFont("fonts/Kazesawa-Bold.ttf", 100, true, true, true);
	this->word = "FIRE";
	int sample_count = 200;
	vector<ofPath> word_path = this->font.getStringAsPoints(word, true, false);
	for (int word_index = 0; word_index < word_path.size(); word_index++) {

		vector<ofPolyline> outline = word_path[word_index].getOutline();
		for (int outline_index = 0; outline_index < outline.size(); outline_index++) {

			outline[outline_index] = outline[outline_index].getResampledByCount(sample_count);
			vector<glm::vec3> vertices = outline[outline_index].getVertices();
			for (int i = 0; i < vertices.size(); i++) {

				this->font_location_list.push_back(vertices[i] + glm::vec3(0, 0, 0));
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	int radius = 3;
	while (this->log_list.size() < 1300) {

		int font_location_index = ofRandom(this->font_location_list.size());
		vector<glm::vec3> log;
		log.push_back(this->font_location_list[font_location_index]);
		this->log_list.push_back(log);
		this->color_list.push_back(this->base_color_list[(int)ofRandom(this->base_color_list.size())]);
		this->life_list.push_back(ofRandom(30, 50));
	}

	for (int i = this->log_list.size() - 1; i >= 0; i--) {

		this->life_list[i] -= 1;
		if (this->life_list[i] < 0) {

			this->log_list.erase(this->log_list.begin() + i);
			this->color_list.erase(this->color_list.begin() + i);
			this->life_list.erase(this->life_list.begin() + i);

			continue;
		}

		auto deg = ofMap(ofNoise(glm::vec4(this->log_list[i].back() * 0.008, ofGetFrameNum() * 0.003)), 0, 1, 180, 360);
		this->log_list[i].push_back(this->log_list[i].back() + glm::vec2(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD)));
		while (this->log_list[i].size() > 100) {

			this->log_list[i].erase(this->log_list[i].begin());
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(180);
	ofTranslate(this->font.stringWidth("FIRE") * -0.5, this->font.stringHeight("FIRE") * 0.5);

	ofSetColor(239);
	this->font.drawStringAsShapes(this->word, 0, 0);

	for (int i = 0; i < this->log_list.size(); i++) {

		ofSetColor(this->color_list[i]);

		if (this->life_list[i] > 30) {

			ofSetLineWidth(1.5);
		}
		else {

			ofSetLineWidth(ofMap(this->life_list[i], 0, 30, 0, 1.5));
		}

		ofBeginShape();
		ofVertices(this->log_list[i]);
		ofEndShape();
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}