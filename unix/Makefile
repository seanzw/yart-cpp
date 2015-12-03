CXX		:= clang++
CXXFLAGS	:= -O3 -std=c++1y
LNKFLAGS	:= -Wl,-allow-multiple-definition

INCLUDES	:= -I../include
LIBS		:= -lfreeimage -lpthread
SRCPATH 	:= ../src
OBJPATH 	:= ./obj
BINPATH 	:= ./bin

OBJECTS		:=				\
	$(OBJPATH)/AdaptivePixelSampler.o	\
	$(OBJPATH)/AdaptivePixelSampler.o    \         
	$(OBJPATH)/AreaLight.o    \                    
	$(OBJPATH)/BBox.o    \                         
	$(OBJPATH)/BidirectionalPathIntegrator.o    \  
	$(OBJPATH)/BSDFCookTorrance.o    \            
	$(OBJPATH)/BSDF.o    \                         
	$(OBJPATH)/Camera.o    \                       
	$(OBJPATH)/DEBUG.o    \                        
	$(OBJPATH)/DirectLightIntegrator.o    \        
	$(OBJPATH)/Film.o    \                         
	$(OBJPATH)/JitteredPixelSampler.o    \         
	$(OBJPATH)/lex.yy.o    \                       
	$(OBJPATH)/main.o    \                         
	$(OBJPATH)/Material.o    \                     
	$(OBJPATH)/Mesh.o    \
	$(OBJPATH)/MonteCarloIntegrator.o    \
	$(OBJPATH)/NormalTriangle.o    \
	$(OBJPATH)/OCTree.o    \
	$(OBJPATH)/PlaneTriangle.o    \
	$(OBJPATH)/PointLight.o    \
	$(OBJPATH)/RayTracer.o    \
	$(OBJPATH)/Sampler.o    \
	$(OBJPATH)/Scene.o    \
	$(OBJPATH)/Sphere.o    \
	$(OBJPATH)/Transform.o    \
	$(OBJPATH)/Triangle.o    \
	$(OBJPATH)/UniformPixelSampler.o    \
	$(OBJPATH)/yart-cpp.tab.o    \

$(BINPATH)/yart: $(OBJECTS)
	$(CXX) $(inputs) -o $(output) $(LIBS) $(LNKFLAGS)

$(OBJPATH)/%.o: $(SRCPATH)/%.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c $(input) -o $(output)

clean:
	rm $(OBJPATH)/*
	rm $(BINPATH)/*


