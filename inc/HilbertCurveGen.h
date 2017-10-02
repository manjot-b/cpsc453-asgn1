#ifndef HILBERT_CURVE_GEN
#define HILBERT_CURVE_GEN

class HilbertCurveGen
{
    public: 
        HilbertCurveGen(int level);
        vector<float> getVerticies() const;
    private:
        vector<float> vertices;
        
};

#endif