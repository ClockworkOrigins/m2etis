/*
 Copyright (2016) Michael Baer, Daniel Bonrath, All rights reserved.

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <stack>
#include <string>
#include <vector>

struct Node {
	int id;
	std::string name;
	uint16_t stage;
	int exclusiveSamples = 0;
	int inclusiveSamples = 0;
	double percent = 0.0;
	std::set<Node *> childs;
};

struct Tree {
	std::set<Node *> nodes;
};

std::map<std::string, Node *> nodeMap;

std::vector<std::string> split(const std::string & str, const std::string & delim) {
	std::vector<std::string> ret;

	size_t n = 0;
	size_t n2 = str.find(delim);

	while (n2 != std::string::npos) {
		std::string s = str.substr(n, n2 - n);
		n = n2 + 1;
		n2 = str.find(delim, n);

		if (!s.empty()) {
			ret.push_back(s);
		}
	}

	if (str.size() - n > 0) {
		ret.push_back(str.substr(n, str.size() - n));
	}

	return ret;
}

int main(int argc, char ** argv) {
	argc--;
	argv++;

	if (argc != 2) {
		std::cerr << "Requiring two arguments: inputFile outputFile" << std::endl;
		return -1;
	}

	Tree tree;

	std::stack<Node *> nodes;

	std::ifstream in(argv[0]);
	if (in.good()) {
		char buf[2048];
		in.getline(buf, 2048);
	}
	while (in.good()) {
		char buf[2048];
		in.getline(buf, 2048);
		if (!in.good()) {
			break;
		}
		std::string line = buf;
		line = std::regex_replace(line, std::regex("(\\\"[0-9]+)\\,([0-9]+\\\")"), "$1.$2");
		std::vector<std::string> vec = split(line, ",");
		Node * n = nullptr;
		if (nodeMap.find(vec[0]) != nodeMap.end()) {
			n = nodeMap[vec[0]];
		} else {
			n = new Node();
			n->name = vec[0];
			n->name = std::regex_replace(n->name, std::regex("\\\""), "");
			n->name = std::regex_replace(n->name, std::regex("\\["), "");
			n->name = std::regex_replace(n->name, std::regex("\\]"), "");
			nodeMap.insert(std::make_pair(vec[0], n));
		}
		try {
			n->stage = uint16_t(std::stoul(vec[10]));
			n->exclusiveSamples += std::stoi(vec[2]);
			n->inclusiveSamples += std::stoi(vec[1]);
		} catch (std::invalid_argument) {
			continue;
		}
		if (n->inclusiveSamples == 0) {
			continue;
		}
		if (tree.nodes.empty()) {
			tree.nodes.insert(n);
		} else {
			if (n->stage > nodes.top()->stage) {
				nodes.top()->childs.insert(n);
			} else if (n->stage <= nodes.top()->stage) {
				while (!nodes.empty() && n != nodes.top() && n->stage <= nodes.top()->stage) {
					nodes.pop();
				}
				if (n->stage == 0) {
					tree.nodes.insert(n);
				} else {
					nodes.top()->childs.insert(n);
				}
			}
		}
		nodes.push(n);
	}

	int samples = 0;
	for (auto p : nodeMap) {
		samples += p.second->inclusiveSamples;
	}
	for (auto p : nodeMap) {
		p.second->percent = p.second->inclusiveSamples / double(samples);
	}

	std::ofstream out(argv[1]);
	out << "digraph {" << std::endl;
	int id = 0;
	for (auto p : nodeMap) {
		for (Node * n : p.second->childs) {
			if (p.second->percent >= 0.01 && n->percent >= 0.01) {
				out << "\t" << id << "[label=\"" << p.second->name << " (" << std::floor(p.second->percent * 100) << "%)" << "\"]" << ";" << std::endl;
			}
		}
		p.second->id = id;
		id++;
	}
	for (auto p : nodeMap) {
		for (Node * n : p.second->childs) {
			if (p.second->percent >= 0.01 && n->percent >= 0.01) {
				out << "\t" << p.second->id << " -> " << n->id << ";" << std::endl;
			}
		}
	}
	out << "}" << std::endl;

	return 0;
}
