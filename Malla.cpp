#include "Malla.hpp"
#define MAXFLOAT 3.40282e+038
Malla::Malla()
{
    mColor = ColorRGB(0,0,0);
    mInicio = true;
	this->funcion = normal;
	SetLimitesX(-100, 100);
	SetLimitesY(-100, 100);
	SetNumberLines(200);
}

Malla::Malla(GLuint matrizid)
{
	SetMatrizID(matrizid);
}

Malla::~Malla()
{
    FreeArray(mMalla);
}

ColorRGB Malla::GetColor()
{
    return mColor;
}

GLvoid Malla::SetColor(const ColorRGB& RGB)
{
    if (mColor == RGB)return;
	mColor = RGB;
}

void Malla::dcRender(glm::mat4 const &m)
{
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, glm::value_ptr(m));
    dcRender(mMalla);
}

void Malla::dcUpdate()
{
    // Create our datapoints, store it as bytes
    if(!mInicio)
        FreeArray(mMalla);
    else
        mInicio = false;
    uint32_t N = (int)mNumberTheLines.x;
    uint32_t M = (int)mNumberTheLines.y;
	vector<Vertex3d> graph(N * M);
	vector<GLfloat> graphF(N * M);
	
	GLfloat Max=-MAXFLOAT,Min = MAXFLOAT;
    GLfloat deltaX = (mLimitesX.y-mLimitesX.x)/N,deltaY = (mLimitesY.y-mLimitesY.x)/M;
    vector<double> Xi(2u);
	for (int i = 0; i < N; i++) {
		for (int j = 0,ID = i * M; j < M; j++) {
			Xi[0] = i*deltaX + mLimitesX.x;
			Xi[1] = j*deltaY + mLimitesY.x;
			float z = funcion(Xi.data(),Xi.size());
			uint64_t Index= ID + j;
            graph[Index].Pos.x = Xi[0];
            graph[Index].Pos.y = Xi[1];
            graph[Index].Pos.z = z;
			graph[Index].coorText = vect3(0,0,0);
            graph[Index].color = mColor;
			graphF[Index] = z;
			if(Max < z)
				Max = z;
			if(Min > z)
				Min = z;
		}
	}
	std::cout << "maximi :" << Max << std::endl;
	GLfloat Scalar = (Max-Min)>0?(mLimitesX.y-mLimitesX.x)/(Max-Min):1;
	this->Scalar = glm::vec3(Max,Min,Scalar);
	for (int i = 0; i < N; i++) {
		for (int j = 0, id = i * N; j < N; j++) {
			uint64_t Index= id + j;
            graph[Index].Pos.z = graphF[Index]*Scalar-(Max+Min)/2 * Scalar;
		}
	}
	// Create two vertex buffer objects
	glGenBuffers(2, mMalla);
	// Tell OpenGL to copy our array to the buffer objects
	glBindBuffer(GL_ARRAY_BUFFER, mMalla[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex3d) * graph.size(), graph.data(), GL_STATIC_DRAW);

	// Create an array of indices into the vertex array that traces both horizontal and vertical lines
	vector<GLuint> indices(N * (M)* 4);
	int i = 0;

	for (int y = 0; y < M; y++) {
		for (int x = 0; x < N-1; x++) {
			indices[i++] = y * M + x;
			indices[i++] = y * M + x + 1;
		}
	}

	for (int x = 0; x < M; x++) {
		for (int y = 0; y < N-1; y++) {
			indices[i++] = y * N + x;
			indices[i++] = (y + 1) * N + x;
		}
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mMalla[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices.size(), indices.data(), GL_STATIC_DRAW);
   
}

void Malla::dcRender(const GLuint *vpo)
{

    uint32_t N = (int)mNumberTheLines.x;
    uint32_t M = (int)mNumberTheLines.y;
    /* Draw the grid using the indices to our vertices using our vertex buffer objects */
	glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vpo[0]);
	glVertexAttribPointer( 0, 3,  GL_FLOAT,GL_FALSE, sizeof(Vertex3d),0);
    glVertexAttribPointer(1,3, GL_FLOAT, GL_FALSE, sizeof(struct Vertex3d),
        (GLvoid*) offsetof(Vertex3d, Vertex3d::color)  // offset
    );

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vpo[1]);;
	glDrawElements(GL_LINES,  N * M * 4, GL_UNSIGNED_INT, 0);

	/* Stop using the vertex buffer object */
	glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void Malla::FreeArray(GLuint *vpo)
{
    glDeleteBuffers(1,&vpo[0]);
}

void Malla::SetFuncion( double (*f)(double * X,int N))
{
    funcion = f;
}

void Malla::SetLimitesX(float xi,float xf)
{
    mLimitesX = glm::vec2(xi,xf);
}

void Malla::SetLimitesY(float yi,float yf)
{
    mLimitesY = glm::vec2(yi,yf);
}

void Malla::SetNumberLines(int32_t N)
{
	if(N < 2)
		mNumberTheLines = glm::vec2(2,2);
	else
    	mNumberTheLines = glm::vec2(N,N);
}


unsigned int Malla::GetNumberLines()
{
	return (uint32_t)mNumberTheLines.x;
}