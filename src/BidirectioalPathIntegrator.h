#ifndef BIDIRECTIONAL_PATH_INTEGRATOR_HEADER
#define BIDIRECTIONAL_PATH_INTEGRATOR_HEADER

#include "Integrator.h"

class BidirectionalPathIntegrator : public Integrator {
public:
    BidirectionalPathIntegrator(int eyePathLen, int lightPathLen)
        : eyePathLen(eyePathLen), lightPathLen(lightPathLen),
        eyePath(eyePathLen), lightPath(lightPathLen) {

    }

    ~BidirectionalPathIntegrator() {

    }

    virtual void prepare(const shared_ptr<Scene> &scene);
    virtual vec3 income(const Ray &r, const shared_ptr<Scene> &scene);
    virtual shared_ptr<Integrator> copy() const {
        return make_shared<BidirectionalPathIntegrator>(eyePathLen, lightPathLen);
    }

private:
    int eyePathLen;
    int lightPathLen;

    /**
     * The interface of the vertex on the path.
     */
    class Vertex {

    protected:
        vec3 a;
    public:
        Vertex() : a(0.0f) {}
        Vertex(vec3 alpha) : a(alpha) {}
        ~Vertex() {}

        virtual pair<Ray, float> sample() = 0;          /* Sample the BRDF, with PSA possiblity. */
        virtual vec3 brdf(const vec3 &out) const = 0;   /* Get the BRDF value. */
        virtual float pdf(const vec3 &out) const = 0;   /* Get the PSA possibility. */
        virtual vec3 point() const = 0;                 /* Get the position of the point. */
        virtual vec3 normal() const = 0;                /* Get the normal of the point. */

        inline vec3 alpha() const { return a; }
    };

    // Struct to represent the vertex along the path.
    class VertexObj : public Vertex {
    private:
        Intersection hit;
        vec3 in;
        float g;   // Geometry term.
        float p;          // Probability of projected solid angle.
        
    public:
        VertexObj(const vec3 &alpha,
            const Intersection &hit,
            const vec3 &in,
            const float geometry,
            const float p
            ) : Vertex(alpha), hit(hit), in(in), g(geometry), p(p) {}
        VertexObj() : hit(NULL, CONST_FAR), in(0.0f), g(0.0f), p(0.0f) {}

        virtual pair<Ray, float> sample() { return hit.m->brdf->sample(hit); }
        virtual vec3 brdf(const vec3 &out) const { return hit.m->brdf->brdf(hit, in, out); }
        virtual float pdf(const vec3 &out) const { return hit.m->brdf->pdf(hit, out); }
        virtual vec3 point() const { return hit.point; }
        virtual vec3 normal() const { return hit.normal; }
        virtual float geometry() const { return g; }
        virtual float possibility() const { return p; }
    };

    // Struct to represent the point on the light.
    class VertexLight : public Vertex {
    public:
        vec3 pos;
        vec3 n;
        
        VertexLight(const vec3 &point, const vec3 &normal, const vec3 &alpha) :
            Vertex(alpha), pos(point), n(normal) {}
        VertexLight() : pos(0.0f), n(0.0f) {}

        virtual pair<Ray, float> sample() {
            cerr << "BidirectionalPathIntegrator::VertexLight: Unimplemented sample method\n";
            exit(1);
        }
        
        virtual vec3 brdf(const vec3 &out) const {
            return vec3(clamp(dot(out, n), CONST_NEAR, 1.0f));
        }

        /* Always cosin sampling the hemisphere. */
        virtual float pdf(const vec3 &out) const {
            return INV_PI;
        }

        virtual vec3 point() const { return pos; }
        virtual vec3 normal() const { return n; }
    };

    // Two paths.
    vector<VertexObj> eyePath;
    vector<VertexObj> lightPath;

    // Calculates the contribution of the connecting edge.
    vec3 connect(const shared_ptr<Scene> &scene,
        const Vertex &v1, const Vertex &v2) const;

    /**
     * Compute the weight of this path.
     * @param iEye: the idx of the vertex on the eye subpath.
     * @param iLight: the idx of the vertex on the light subpath.
     * @return float: the weight of this path.
     */
    float computeWeight(int iEye, int iLight) const;

};

#endif