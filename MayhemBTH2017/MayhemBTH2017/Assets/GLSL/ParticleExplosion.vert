#version 420


in vec3 inPos;
in vec3 inDir;
in vec4 inCol;

in float inLife;
in float inSize;

out vec3 outPos;
out vec3 outDir;
out vec4 outCol;

out float outLife;
out float outSize;


void main()
{

	int seed=gl_VertexID;
	

	outPos=inPos+0.01/(normalize(inDir));

	outCol=inCol;
	outCol.a=inCol.a-0.0001f;
	outCol.r+=0.002f;
	outCol.g=seed;
	outCol.b=0.0f;

	
	outDir=inDir;
	outLife=inLife+0.05f;
	outSize=inSize+4f;

	
	

	



	outSize=inSize;

}

