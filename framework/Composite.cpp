#include "Composite.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <limits>

bool Composite::intersect(const Ray& ray,
                          HitPoint& hp,
                          const glm::mat4& parentWorldToLocal,
                          const glm::mat3& parentNormalMat) const {
    // Compose this node's transform with parent chain
    glm::mat4 worldToLocal = this->invTransform() * parentWorldToLocal;
    glm::mat3 normalMat = glm::mat3(glm::transpose(worldToLocal)); // since worldToLocal is inverse of localToWorld
    // Trace into children and pick closest hit
    bool hitAny = false;
    for(const auto& ch : children_){
        if(!ch) continue;
        HitPoint tmp = hp;
        if(ch->intersect(ray, tmp, worldToLocal, normalMat)){
            if(tmp.hit && tmp.t < hp.t){
                hp = tmp;
                hitAny = true;
            }
        }
    }
    return hitAny;
}
