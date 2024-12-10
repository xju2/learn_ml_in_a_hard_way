#include <vector>
#include <cmath>
#include <algorithm>
#include <queue>
#include <utility> // for std::pair
#include <iostream>

void frnnBrutalForce(
    const std::vector<float>& embedFeatures,
    std::vector<int64_t>& senders,
    std::vector<int64_t>& receivers,
    int64_t numSpacepoints,
    int embeddingDim,
    float rVal,
    int kVal
) {
    // Helper lambda to calculate squared distance between two points
    auto squaredDistance = [&](int64_t i, int64_t j) {
        float dist = 0.0;
        for (int d = 0; d < embeddingDim; ++d) {
            float diff = embedFeatures[i * embeddingDim + d] - embedFeatures[j * embeddingDim + d];
            dist += diff * diff;
        }
        return dist;
    };

    // Radius squared to avoid taking square root repeatedly
    float radiusSquared = rVal * rVal;

    for (int64_t i = 0; i < numSpacepoints; ++i) {
        // Min-heap (priority queue) to store nearest neighbors
        std::priority_queue<std::pair<float, int64_t>> nearestNeighbors;

        for (int64_t j = 0; j < numSpacepoints; ++j) {
            if (i == j || j <= i) continue; // Skip self and enforce i < j symmetry

            float distSquared = squaredDistance(i, j);

            // If within radius, consider it as a candidate
            if (distSquared <= radiusSquared) {
                nearestNeighbors.push({distSquared, j});
                // Maintain top k neighbors in the heap
                if (nearestNeighbors.size() > kVal) {
                    nearestNeighbors.pop();
                }
            }
        }

        // Add edges to senders and receivers
        while (!nearestNeighbors.empty()) {
            auto [dist, neighbor] = nearestNeighbors.top();
            nearestNeighbors.pop();
            senders.push_back(i);
            receivers.push_back(neighbor);
        }
    }
}



struct KDNode {
    int64_t index; // Index of the point in the original data
    KDNode* left;  // Left child
    KDNode* right; // Right child
};
/***
class KDTree {
    const std::vector<float>& points; // Reference to embedding features
    int embeddingDim;

    // Helper function to build the KD-tree recursively
    KDNode* buildTree(std::vector<int64_t>& indices, int depth) {
        if (indices.empty()) return nullptr;

        int axis = depth % embeddingDim; // Current splitting axis
        // Sort indices by the current axis
        std::sort(indices.begin(), indices.end(), [&](int64_t a, int64_t b) {
            return points[a * embeddingDim + axis] < points[b * embeddingDim + axis];
        });

        // Choose median as the root
        size_t medianIdx = indices.size() / 2;
        KDNode* node = new KDNode{indices[medianIdx], nullptr, nullptr};

        // Recursively build left and right subtrees
        std::vector<int64_t> leftIndices(indices.begin(), indices.begin() + medianIdx);
        std::vector<int64_t> rightIndices(indices.begin() + medianIdx + 1, indices.end());
        node->left = buildTree(leftIndices, depth + 1);
        node->right = buildTree(rightIndices, depth + 1);

        return node;
    }

    // Helper function to search for neighbors within a radius
    void radiusSearch(KDNode* node, const std::vector<float>& queryPoint, float radiusSquared, int depth,
                      std::vector<int64_t>& neighbors, int64_t queryIdx) {
        if (!node) return;

        int axis = depth % embeddingDim;
        float distSquared = 0.0;
        for (int d = 0; d < embeddingDim; ++d) {
            float diff = queryPoint[d] - points[node->index * embeddingDim + d];
            distSquared += diff * diff;
        }

        if (distSquared <= radiusSquared && node->index > queryIdx) {
            neighbors.push_back(node->index);
        }

        float diffAxis = queryPoint[axis] - points[node->index * embeddingDim + axis];
        float diffAxisSquared = diffAxis * diffAxis;

        // Recursively search the child nodes
        if (diffAxis <= 0) {
            radiusSearch(node->left, queryPoint, radiusSquared, depth + 1, neighbors, queryIdx);
            if (diffAxisSquared <= radiusSquared) {
                radiusSearch(node->right, queryPoint, radiusSquared, depth + 1, neighbors, queryIdx);
            }
        } else {
            radiusSearch(node->right, queryPoint, radiusSquared, depth + 1, neighbors, queryIdx);
            if (diffAxisSquared <= radiusSquared) {
                radiusSearch(node->left, queryPoint, radiusSquared, depth + 1, neighbors, queryIdx);
            }
        }
    }

public:
    KDNode* root;

    KDTree(const std::vector<float>& embedFeatures, int embeddingDim)
        : points(embedFeatures), embeddingDim(embeddingDim), root(nullptr) {}

    void build(int64_t numPoints) {
        std::vector<int64_t> indices(numPoints);
        for (int64_t i = 0; i < numPoints; ++i) indices[i] = i;
        root = buildTree(indices, 0);
    }

    void query(const std::vector<float>& queryPoint, float radiusSquared, std::vector<int64_t>& neighbors, int64_t queryIdx) {
        radiusSearch(root, queryPoint, radiusSquared, 0, neighbors, queryIdx);
    }
};

void buildEdges(
    const std::vector<float>& embedFeatures,
    std::vector<int64_t>& senders,
    std::vector<int64_t>& receivers,
    int64_t numSpacepoints,
    int embeddingDim,
    float rVal,
    int kVal
) {
    float radiusSquared = rVal * rVal;

    // Build the KD-tree
    KDTree kdTree(embedFeatures, embeddingDim);
    kdTree.build(numSpacepoints);

    // Perform fixed radius neighbor search
    for (int64_t i = 0; i < numSpacepoints; ++i) {
        std::vector<float> queryPoint(embedFeatures.begin() + i * embeddingDim,
                                      embedFeatures.begin() + (i + 1) * embeddingDim);
        std::vector<int64_t> neighbors;

        kdTree.query(queryPoint, radiusSquared, neighbors, i);

        // Select up to kVal nearest neighbors
        std::sort(neighbors.begin(), neighbors.end(), [&](int64_t a, int64_t b) {
            float distA = 0.0, distB = 0.0;
            for (int d = 0; d < embeddingDim; ++d) {
                float diffA = queryPoint[d] - embedFeatures[a * embeddingDim + d];
                float diffB = queryPoint[d] - embedFeatures[b * embeddingDim + d];
                distA += diffA * diffA;
                distB += diffB * diffB;
            }
            return distA < distB;
        });

        for (int64_t j = 0; j < std::min(kVal, static_cast<int64_t>(neighbors.size())); ++j) {
            senders.push_back(i);
            receivers.push_back(neighbors[j]);
        }
    }
}

****/

int main() {
    // Example data
    std::vector<float> embedFeatures = {
        0.0, 0.0,
        1.0, 0.0,
        0.0, 1.0,
        1.0, 1.0,
        0.5, 0.5
    };
    int64_t numSpacepoints = 5;
    int embeddingDim = 2;
    float rVal = 1.5;
    int kVal = 2;

    std::vector<int64_t> senders, receivers;

    frnnBrutalForce(embedFeatures, senders, receivers, numSpacepoints, embeddingDim, rVal, kVal);

    // Print results
    for (size_t i = 0; i < senders.size(); ++i) {
        std::cout << "Edge: " << senders[i] << " -> " << receivers[i] << std::endl;
    }

    return 0;
}
