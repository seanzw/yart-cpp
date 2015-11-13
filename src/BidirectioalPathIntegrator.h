#ifndef BIDIRECTIONAL_PATH_INTEGRATOR_HEADER
#define BIDIRECTIONAL_PATH_INTEGRATOR_HEADER

#include "Integrator.h"

class BidirectionalPathIntegrator : public Integrator {
public:
    BidirectionalPathIntegrator(int eyePathLen, int lightPathLen)
        : eyePathLen(eyePathLen), lightPathLen(lightPathLen),
        eyePath(eyePathLen), lightPath(lightPathLen + 1),
        eyePathBuf(eyePathLen), lightPathBuf(lightPathLen) {

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
    public:
        const vec3 alpha;       // Cumulative subpath contribution.
        const bool isDelta;     // True if this vertex's brdf contains a delta function.
        const vec3 point;       // The position of the vertex.
        const vec3 normal;      // The normal at the vertex.
        const float geometry;   // The geometry term with the previous vertex.

        const float probPrev;   // Previous probability: previous vertex's probFWD.
        float probBWD;          // Backward probability: out samples in.

        Vertex(const vec3 &alpha,
            bool isDelta,
            const vec3 &point,
            const vec3 &normal,
            float geometry,
            float probPrev
            ) : alpha(alpha), isDelta(isDelta), point(point), normal(normal), geometry(geometry), probPrev(probPrev) {}
        Vertex() :
            alpha(0.0f), isDelta(false), point(0.0f), normal(0.0f), geometry(1.0f), probPrev(1.0f) {}
        ~Vertex() {}

        /**
         * Samples the BSDF.
         * Updates the outgoing direction and backward probability.
         *
         * @return pair of sampled Ray and its PSA probability.
         */
        virtual pair<Ray, float> sample() = 0;
        virtual vec3 brdf(const vec3 &out) const = 0;       /* Get the BRDF value. */
        virtual float pdf(const vec3 &out) const = 0;       /* Get the PSA possibility, in to out. */
        virtual float pdfInv(const vec3 &out) const = 0;    /* Get the inverse PSA probability, out to in*/
    };

    // Struct to represent the vertex along the path.
    class VertexObj : public Vertex {
    public:
        const Intersection hit;
        const vec3 in;      // Incoming direction.
        vec3 out;           // Outgoing direction.

        VertexObj(const vec3 &alpha,
            const Intersection &hit,
            const vec3 &in,
            const float geometry,
            const float probPrev,
            bool isDelta
            ) : Vertex(alpha, isDelta, hit.point, hit.normal, geometry, probPrev), hit(hit), in(in) {}
        VertexObj() : hit(NULL, NULL, CONST_FAR), in(0.0f) {}

        virtual pair<Ray, float> sample() {
            auto ret = hit.m->brdf->sample(hit, in);
            out = ret.first.d;         // Updates outgoing direction.
            probBWD = pdfInv(out);     // Updates backward probability.
            return ret;
        }

        virtual vec3 brdf(const vec3 &out) const { return hit.m->brdf->brdf(hit, in, out); }
        virtual float pdf(const vec3 &out) const { return hit.m->brdf->pdf(hit, in, out); }
        virtual float pdfInv(const vec3 &out) const { return hit.m->brdf->pdf(hit, -out, -in); }
    };

    // Struct to represent the point on the light.
    class VertexLight : public Vertex {
    public:

        vec3 tangent;
        vec3 cotangent;
        
        VertexLight(const vec3 &point, const vec3 &normal, const vec3 &alpha, float probPrev) :
            Vertex(alpha, false, point, normal, 1.0f, probPrev) {
            auto localCoordinate = buildLocalCoordinate(normal);
            tangent = localCoordinate.first;
            cotangent = localCoordinate.second;
        }

        virtual pair<Ray, float> sample() {
            auto d = Sampler::cosinSampleHemisphere(normal, tangent, cotangent);
            return make_pair(Ray(point, d, CONST_NEAR, CONST_FAR), INV_PI);
        }
        
        virtual vec3 brdf(const vec3 &out) const {
            return vec3(clamp(dot(out, normal), CONST_EPSILON, 1.0f));
        }

        /* Always cosin sampling the hemisphere. */
        virtual float pdf(const vec3 &out) const {
            return INV_PI;
        }

        /* Always cosin sampling the hemisphere. */
        virtual float pdfInv(const vec3 &out) const {
            return INV_PI;
        }
    };

    /* Two paths. */
    vector<Vertex *> eyePath;
    vector<Vertex *> lightPath;
    vector<VertexObj> eyePathBuf;
    vector<VertexObj> lightPathBuf;

    /* Add an object vertex to the buffer and the path. */
    inline void pushVertexObj(vector<Vertex *> &path, vector<VertexObj> &buf,
        const vec3 &alpha,
        const Intersection &hit,
        const vec3 &in,
        const float geometry,
        const float probPrev,
        bool isDelta
        ) {
        buf.emplace_back(alpha, hit, in, geometry, probPrev, isDelta);
        path.emplace_back(&buf[buf.size() - 1]);
    }

    /**
     * Calculates the contribution of the connecting edge.
     *
     * @param scene for shadow test.
     * @param v1 vertex 1
     * @param v2 vertex 2
     * @return float the contribution.
     */
    vec3 connect(const shared_ptr<Scene> &scene,
        const Vertex *v1, const Vertex *v2) const;

    /**
    * Handle the special case when the eye subpath accidently intersects a light.
    * Connects the last vertex on the eye subpath with the vertex on the light
    * and returns the radiance.
    *
    * @param vertexLight: vertex on the light.
    * @param toLight: unit vector to the light vertex.
    * @param t: distance to light.
    * @param probFWD: the forward probability.
    * @return Vec3 for the radiance.
    */
    inline vec3 computeRadianceZeroLightVertex(const VertexLight &vertexLight,
        const vec3 &toLight, float t, float probFWD) const;

    /**
     * Compute the weight of the path of zero light vertex.
     *
     * @param vertexLight the light vertex.
     * @param probFWD the probability this situation happened.
     * @return float the weight.
     */
    float computeWeightZeroLightVertex(const VertexLight &vertexLight,
        const vec3 &toLight,
        float t,
        float probFWD,
        float g
        ) const;
    
    /**
     * Handle the general case.
     * Connects the every vertex on the eye subpath with every vertex on the light subpath
     * and returns the radiance.
     *
     * @param scene for shadow test.
     * @return vec3 for the radiance.
     */
    inline vec3 computeRadianceGeneral(const shared_ptr<Scene> &scene) const;

    /**
     * Compute the weight of this path in general case.
     *
     * @param iEye: the idx of the vertex on the eye subpath.
     * @param iLight: the idx of the vertex on the light subpath.
     * @return float: the weight of this path.
     */
    float computeWeightGeneral(int iEye, int iLight) const;

};

#endif