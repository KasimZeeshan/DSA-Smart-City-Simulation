
// ===================== Graph.h FULL IMPLEMENTATION) =====================
#pragma once
#include<iostream>
using namespace std;
#include <string>
#include <fstream>
#include "Types.h"

const int MAX_NODES = 300;
const int MAX_EDGES_PER_NODE = 20;

struct Edge {
    string to;
    int weight;
};

struct GraphNode {
    string id;
    Stop stopInfo;
    Edge edges[MAX_EDGES_PER_NODE];
    int edgeCount;
};

class Graph {
public:
    Graph() {
        nodeCount = 0;
    }

    // -------- Add a node --------
    void addNode(const string& nodeId, const Stop& stop) {
        if (nodeCount >= MAX_NODES) return;
        for (int i = 0; i < nodeCount; i++) {
            if (nodes[i].id == nodeId) return;
        }
        nodes[nodeCount].id = nodeId;
        nodes[nodeCount].stopInfo = stop;
        nodes[nodeCount].edgeCount = 0;
        nodeCount++;
    }

    // -------- Check existence --------
    bool hasNode(const string& nodeId) const {
        return findNodeIndex(nodeId) != -1;
    }

    // -------- Add edge --------
    void addEdge(const string& from, const string& to, int weight) {
        int u = findNodeIndex(from);
        int v = findNodeIndex(to);
        if (u == -1 || v == -1) return;

        if (nodes[u].edgeCount < MAX_EDGES_PER_NODE) {
            nodes[u].edges[nodes[u].edgeCount].to = to;
            nodes[u].edges[nodes[u].edgeCount].weight = weight;
            nodes[u].edgeCount++;
        }
        if (nodes[v].edgeCount < MAX_EDGES_PER_NODE) {
            nodes[v].edges[nodes[v].edgeCount].to = from;
            nodes[v].edges[nodes[v].edgeCount].weight = weight;
            nodes[v].edgeCount++;
        }
    }

    // -------- Get Stop info --------
    Stop getStop(const string& id) const {
        int idx = findNodeIndex(id);
        if (idx == -1) return Stop();
        return nodes[idx].stopInfo;
    }

    // -------- Compute coordinate distance manually --------
    double coordDistance(const Coordinate& a, const Coordinate& b) const {
        double dx = a.lat - b.lat;
        double dy = a.lon - b.lon;
        return dx * dx + dy * dy;
    }

    // -------- Nearest Node --------
    string nearestNodeByCoord(const Coordinate& c) {
        if (nodeCount == 0) return "";
        double best = coordDistance(c, nodes[0].stopInfo.coord);
        string bestId = nodes[0].id;

        for (int i = 1; i < nodeCount; i++) {
            double d = coordDistance(c, nodes[i].stopInfo.coord);
            if (d < best) {
                best = d;
                bestId = nodes[i].id;
            }
        }
        return bestId;
    }

    // -------- Dijkstra--------
    string* dijkstra(const string& src, const string& dest, int& pathSize) {
        pathSize = 0;
        int start = findNodeIndex(src);
        int end = findNodeIndex(dest);
        if (start == -1 || end == -1) return nullptr;

		const int INF = 1000000000; // large number
        int dist[MAX_NODES];
        bool visited[MAX_NODES];
        int parent[MAX_NODES];

        for (int i = 0; i < nodeCount; i++) {
            dist[i] = INF;
            visited[i] = false;
            parent[i] = -1;
        }
        dist[start] = 0;

        for (int k = 0; k < nodeCount; k++) {
            int u = -1;
            for (int i = 0; i < nodeCount; i++) {
                if (!visited[i] && (u == -1 || dist[i] < dist[u])) u = i;
            }
            if (dist[u] == INF) break;
            visited[u] = true;

            for (int i = 0; i < nodes[u].edgeCount; i++) {
                string nxt = nodes[u].edges[i].to;
                int w = nodes[u].edges[i].weight;
                int v = findNodeIndex(nxt);
                if (v == -1) continue;

                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    parent[v] = u;
                }
            }
        }

        if (dist[end] == INF) return nullptr;

        string tempPath[MAX_NODES];
        int t = 0;
        int cur = end;
        while (cur != -1) {
            tempPath[t++] = nodes[cur].id;
            cur = parent[cur];
        }

        pathSize = t;
        string* finalPath = new string[t];
        for (int i = 0; i < t; i++) {
            finalPath[i] = tempPath[t - i - 1];
        }
        return finalPath;
    }

private:
    GraphNode nodes[MAX_NODES];
    int nodeCount;

    int findNodeIndex(const string& id) const {
        for (int i = 0; i < nodeCount; i++) if (nodes[i].id == id) return i;
        return -1;
    }
};
