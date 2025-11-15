#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

// sb7.h 헤더 파일을 포함시킨다.
#include <sb7.h>
#include <vmath.h>
#include <shader.h>
#include <vector>
#include <iostream>

//사운드 라이브러리
#include <Windows.h> 
#include <conio.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")



#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Model.h"

using namespace std;




// sb6::application을 상속받는다.
class my_application : public sb7::application
{
public:
	// 쉐이더 프로그램 컴파일한다.
	GLuint compile_shader(const char* vs_file, const char* fs_file)
	{
		// 버텍스 쉐이더를 생성하고 컴파일한다.
		GLuint vertex_shader = sb7::shader::load(vs_file, GL_VERTEX_SHADER);

		// 프래그먼트 쉐이더를 생성하고 컴파일한다.
		GLuint fragment_shader = sb7::shader::load(fs_file, GL_FRAGMENT_SHADER);

		// 프로그램을 생성하고 쉐이더를 Attach시키고 링크한다.
		GLuint program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);
		glLinkProgram(program);

		// 이제 프로그램이 쉐이더를 소유하므로 쉐이더를 삭제한다.
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		return program;
	}

	GLuint compile_shader2(const char* vs_file, const char* fs_file)
	{
		// 버텍스 쉐이더를 생성하고 컴파일한다.
		GLuint vertex_shader2 = sb7::shader::load(vs_file, GL_VERTEX_SHADER);

		// 프래그먼트 쉐이더를 생성하고 컴파일한다.
		GLuint fragment_shader2 = sb7::shader::load(fs_file, GL_FRAGMENT_SHADER);

		// 프로그램을 생성하고 쉐이더를 Attach시키고 링크한다.
		GLuint program2 = glCreateProgram();
		glAttachShader(program2, vertex_shader2);
		glAttachShader(program2, fragment_shader2);
		glLinkProgram(program2);

		// 이제 프로그램이 쉐이더를 소유하므로 쉐이더를 삭제한다.
		glDeleteShader(vertex_shader2);
		glDeleteShader(fragment_shader2);

		return program2;
	}
	// 포스트 프로세싱을 사용하기 위한 컴파일 함수.
	GLuint compile_shader_screen(const char* vs_file, const char* fs_file)
	{
		// 버텍스 쉐이더를 생성하고 컴파일한다.
		GLuint vertex_shader3 = sb7::shader::load(vs_file, GL_VERTEX_SHADER);

		// 프래그먼트 쉐이더를 생성하고 컴파일한다.
		GLuint fragment_shader3 = sb7::shader::load(fs_file, GL_FRAGMENT_SHADER);

		// 프로그램을 생성하고 쉐이더를 Attach시키고 링크한다.
		GLuint program3 = glCreateProgram();
		glAttachShader(program3, vertex_shader3);
		glAttachShader(program3, fragment_shader3);
		glLinkProgram(program3);

		// 이제 프로그램이 쉐이더를 소유하므로 쉐이더를 삭제한다.
		glDeleteShader(vertex_shader3);
		glDeleteShader(fragment_shader3);

		return program3;
	}

	//텍스쳐 로드 함수
	void loadTexture(const char* filepath, GLuint texture) {
		glBindTexture(GL_TEXTURE_2D, texture);
		int width, height, nrChannels;
		unsigned char* data = stbi_load(filepath, &width, &height, &nrChannels, 0);
		if (data) {
			GLenum format;
			if (nrChannels == 1)
				format = GL_RED;
			else if (nrChannels == 3) // R, G, B (ex. jpg)
				format = GL_RGB;
			else if (nrChannels == 4) // PNG
				format = GL_RGBA;
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			stbi_image_free(data);
		}
	}



	// 애플리케이션 초기화 수행한다.
	virtual void startup()
	{
		// 클래스 내부 변수 초기화
		initValues();

		// 쉐이더 프로그램 컴파일 및 연결
		shader_program = compile_shader("simple_phong_vs.glsl", "simple_phong_fs.glsl");
		shader_program2 = compile_shader2("basic_texturing_vs.glsl", "basic_texturing_fs.glsl");
		shader_program_screen = compile_shader_screen("screen_filtering_vs.glsl", "screen_filtering_fs.glsl");

		stbi_set_flip_vertically_on_load(true);

		
		//시골 벌레소리 재생
		PlaySound(TEXT("Night.wav"), NULL, SND_ASYNC || SND_LOOP);

		// 첫 번째 객체 정의 : OBJ 모델  --------------------------------------------------
		objModel.init();
		//objModel.loadOBJ("squid_girl.obj");
		//objModel.loadDiffuseMap("squid_girl.png");
		objModel.loadOBJ("house.obj");
		objModel.loadDiffuseMap("house.png");
		//objModel.loadSpecularMap("house.png");


		//: Obj Mushroom 모델  --------------------------------------------------
		cuteModel.init();
		cuteModel.loadOBJ("mushroom.obj");
		cuteModel.loadDiffuseMap("mushroom.png");

		//: Obj Squidgirl 모델  --------------------------------------------------
		squidgirlModel.init();
		squidgirlModel.loadOBJ("squid_girl.obj");
		squidgirlModel.loadDiffuseMap("squid_girl.png");
		squidgirlModel.loadSpecularMap("squid_girl.png");
		// : 피라미드 --------------------------------------------------
		// 피라미드 점들의 위치와 컬러, 텍스처 좌표를 정의한다.
		GLfloat pyramid_vertices[] = {
			1.0f, 0.0f, -1.0f,    // 우측 상단
			-1.0f, 0.0f, -1.0f,   // 좌측 상단
			-1.0f, 0.0f, 1.0f,    // 좌측 하단
			1.0f, 0.0f, 1.0f,     // 우측 하단
			0.0f, 1.0f, 0.0f,      // 상단 꼭지점
			0.0f, -1.0f, 0.0f,      // 하단 꼭지점
		};

		// 삼각형으로 그릴 인덱스를 정의한다.
		GLuint pyramid_indices[] = {
			4, 0, 1,
			4, 1, 2,
			4, 2, 3,
			4, 3, 0,

			5, 1, 0,
			5, 2, 1,
			5, 3, 2,
			5, 0, 3,
		};

		pyramidModel.init();
		pyramidModel.setupMesh(6, pyramid_vertices);
		pyramidModel.setupIndices(24, pyramid_indices);

		// 두 번째 객체 정의 : StarBackground 배경화면 --------------------------------------------------
		// 점들의 위치와 컬러, 텍스처 좌표를 정의한다.

		glGenVertexArrays(7, VAO);
		glBindVertexArray(VAO[0]);
		GLfloat star_vertices[] = {
			//앞면
				-0.25f, 0.25f, 0.25f,1.0f,0.0f,0.0f, 0.0f, 1.0f,
				-0.25f, -0.25f, 0.25f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
				0.25f, -0.25f, 0.25f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
				0.25f, 0.25f, 0.25f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,

				//오른쪽 면
				 0.25f, 0.25f, 0.25f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
				 0.25f, -0.25f, 0.25f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
				 0.25f, -0.25f, -0.25f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
				 0.25f, 0.25f, -0.25f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,


				 //뒷면
				  -0.25f, 0.25f, -0.25f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
				 -0.25f, -0.25f, -0.25f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
				 0.25f, -0.25f, -0.25f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
				 0.25f, 0.25f, -0.25f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,

				 //왼쪽면
				  -0.25f, 0.25f, 0.25f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
				  -0.25f, -0.25f, 0.25f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
				 -0.25f, -0.25f, -0.25f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
				 -0.25f, 0.25f, -0.25f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,

				 //위쪽면
				 -0.25f, 0.25f, -0.25f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
				 -0.25f, 0.25f, 0.25f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
				 0.25f, 0.25f,0.25f,  0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
				 0.25f,0.25f,-0.25f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,

				 //아랫면
				 -0.25f, -0.25f, -0.25f, 1.0f,1.0f,1.0f, 0.0f, 1.0f,
				 -0.25f, -0.25f, 0.25f, 1.0f,1.0f,1.0f, 0.0f, 0.0f,
				 0.25f, -0.25f,0.25f,1.0f,1.0f,1.0f, 1.0f, 0.0f,
				 0.25f,-0.25f,-0.25f,1.0f,1.0f,1.0f, 1.0f, 1.0f
			
		};

		GLint star_indices[] = {
			//앞면
			0, 2, 1, // 첫번째 삼각형
			0, 3, 2, // 두번째 삼각형

			//오른쪽 면
			4, 6, 5,
			4, 7, 6,

			//뒷면
			8, 10, 11,
			8, 9, 10,

			//왼쪽면
			12, 13, 14,
			12, 14, 15,

			//윗면
			16, 18, 17,
			16, 19, 18,

			//아랫면
			20, 21, 22,
			20, 22, 23
		};
		
		// VBO 선언
		GLuint VBO[7];
		glGenBuffers(7, VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(star_vertices), star_vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		//EBO 선언 
		GLuint EBO[7];
		glGenBuffers(7, EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(star_indices), star_indices, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		//텍스쳐 생성----------------------------------------------------------------
		glGenTextures(10, texture);
		const char* filenames[] = {
			"Star.png", "Manual.jpg", "2.jpg", "5.jpg", "7.jpg", "WoodDoorLock.png" , "EndingTexture.png", "houseNormal.png", "squidgirl_Normal.png", "mushroomNormal.png"
		};
		for (int i = 0; i < 10; i++) {
			loadTexture(filenames[i], texture[i]);
		}


		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//사용 설명서 ------------------------------------------------
		glBindVertexArray(VAO[1]);
		GLfloat manual_vertices[] = {
			//앞면
				-0.4f, 0.25f, 0.25f,1.0f,0.0f,0.0f, 1.0f, 1.0f,
				-0.4f, -0.25f, 0.25f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
				0.4f, -0.25f, 0.25f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
				0.4f, 0.25f, 0.25f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f

		};

		GLint manual_indices[] = {
			//앞면
			0, 2, 1, // 첫번째 삼각형
			0, 3, 2, // 두번째 삼각형
		};

		// VBO 선언
		glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(manual_vertices), manual_vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		//EBO 선언 
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(manual_indices), manual_indices, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		//숫자 2  ------------------------------------------------
		glBindVertexArray(VAO[2]);
		GLfloat number2_vertices[] = {
			//앞면
				-0.15f, 0.25f, 0.25f,1.0f,0.0f,0.0f, 1.0f, 1.0f,
				-0.15f, -0.25f, 0.25f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
				0.15f, -0.25f, 0.25f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
				0.15f, 0.25f, 0.25f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f

		};

		GLint number2_indices[] = {
			//앞면
			0, 2, 1, // 첫번째 삼각형
			0, 3, 2, // 두번째 삼각형
		};

		// VBO 선언
		glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(number2_vertices), number2_vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		//EBO 선언 
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[2]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(number2_indices), number2_indices, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		//숫자 5 ------------------------------------------------
		glBindVertexArray(VAO[3]);
		GLfloat number5_vertices[] = {
			//앞면
				-0.15f, 0.25f, 0.25f,1.0f,0.0f,0.0f, 1.0f, 1.0f,
				-0.15f, -0.25f, 0.25f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
				0.15f, -0.25f, 0.25f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
				0.15f, 0.25f, 0.25f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f

		};

		GLint number5_indices[] = {
			//앞면
			0, 2, 1, // 첫번째 삼각형
			0, 3, 2, // 두번째 삼각형
		};

		// VBO 선언
		glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(number5_vertices), number5_vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		//EBO 선언 
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[3]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(number5_indices), number5_indices, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		//숫자 5 ------------------------------------------------
		glBindVertexArray(VAO[4]);
		GLfloat number7_vertices[] = {
			//앞면
				-0.15f, 0.25f, 0.25f,1.0f,0.0f,0.0f, 1.0f, 1.0f,
				-0.15f, -0.25f, 0.25f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
				0.15f, -0.25f, 0.25f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
				0.15f, 0.25f, 0.25f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f

		};

		GLint number7_indices[] = {
			//앞면
			0, 2, 1, // 첫번째 삼각형
			0, 3, 2, // 두번째 삼각형
		};

		// VBO 선언
		glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(number7_vertices), number7_vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		//EBO 선언 
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[4]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(number7_indices), number7_indices, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		//DoorLock ------------------------------------------------
		glBindVertexArray(VAO[5]);
		GLfloat doorlock_vertices[] = {
			//앞면
				-0.35f, 0.25f, 0.25f,1.0f,0.0f,0.0f, 1.0f, 1.0f,
				-0.35f, -0.25f, 0.25f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
				0.35f, -0.25f, 0.25f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
				0.35f, 0.25f, 0.25f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f

		};

		GLuint doorlock_indices[] = {
			//앞면
			0, 2, 1, // 첫번째 삼각형
			0, 3, 2, // 두번째 삼각형
		};
		glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(doorlock_vertices), doorlock_vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		//EBO 선언 
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[5]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(doorlock_indices), doorlock_indices, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		//엔딩 ------------------------------------------------
		glBindVertexArray(VAO[6]);
		GLfloat Ending_vertices[] = {
			//앞면
				-0.4f, 0.25f, 0.25f,1.0f,0.0f,0.0f, 1.0f, 1.0f,
				-0.4f, -0.25f, 0.25f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
				0.4f, -0.25f, 0.25f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
				0.4f, 0.25f, 0.25f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f

		};

		GLint Ending_indices[] = {
			//앞면
			0, 2, 1, // 첫번째 삼각형
			0, 3, 2, // 두번째 삼각형
		};

		// VBO 선언
		glBindBuffer(GL_ARRAY_BUFFER, VBO[6]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Ending_vertices), Ending_vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		//EBO 선언 
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[6]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Ending_indices), Ending_indices, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		//  Post-processing ------------------------------------------------------

		GLfloat screenVertices[] = {
			-1.0f, 1.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f,
			1.0f, -1.0f, 1.0f, 0.0f,

			-1.0f, 1.0f, 0.0f, 1.0f,
			1.0f, -1.0f, 1.0f, 0.0f,
			1.0f, 1.0f, 1.0f, 1.0f
		};

		glGenVertexArrays(1, &VAO_screen);
		glGenBuffers(1, &VBO_screen);
		glBindVertexArray(VAO_screen);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_screen);

		glBufferData(GL_ARRAY_BUFFER, sizeof(screenVertices), screenVertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);




		//FrameBuffer---------------------------------------------------------

		glGenFramebuffers(1, &FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		// color buffer 텍스처 생성 및 연결
		glGenTextures(1, &FBO_texture);
		glBindTexture(GL_TEXTURE_2D, FBO_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, info.windowWidth, info.windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBO_texture, 0);
		//for depth&stencil buffer
		GLuint RBO;
		glGenRenderbuffers(1, &RBO);
		glBindRenderbuffer(GL_RENDERBUFFER, RBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, info.windowWidth, info.windowHeight);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
		// Framebuffer가 정상적으로 만들어 졌는지 확인
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			glfwTerminate();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		glEnable(GL_MULTISAMPLE);
	}

	// 애플리케이션 끝날 때 호출된다.
	virtual void shutdown()
	{
		glDeleteProgram(shader_program);
	}

	// 렌더링 virtual 함수를 작성해서 오버라이딩한다.
	virtual void render(double currentTime)
	{
		if (!pause)
			animationTime += currentTime - previousTime;
		
		previousTime = currentTime;

		//updateCamer() 호출을 하여 각 방향키를 눌렀을 때 자연스럽게 움직이도록함.
		updateCamera();
		//1인칭 카메라 y축 위치 고정시키기 
		if (cameraPos[1]>0 || cameraPos[1] <0){
			cameraPos[1] = 0;
		}
		//제한구역설정 > cameraPos의 주소값을 넘겨줌.
		restrictedArea(&cameraPos);

		//doorlockOn 변수가 true가 될 시 카메라 위치 반환
		if (doorlockOn == true) {
			doorlockCheck(&cameraPos, &cameraFront);
		}


		//const GLfloat color[] = { (float)sin(currentTime) * 0.5f + 0.5f, (float)cos(currentTime) * 0.5f + 0.5f, 0.0f, 1.0f };
		const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, black);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);


		// 카메라 매트릭스 계산
		float distance = 5.f;
		
		//vmath::vec3 eye((float)cos(animationTime*0.3f)*distance, 1.0, (float)sin(animationTime*0.3f)*distance);
		//vmath::vec3 eye(0.0f, 1.0f, distance);
		//vmath::vec3 center(0.0, 0.0, 0.0);
		//vmath::vec3 up(0.0, 1.0, 0.0);
		vmath::mat4 lookAt = vmath::lookat(cameraPos, cameraPos + cameraFront, cameraUp);
		vmath::mat4 projM = vmath::perspective(fov, (float)info.windowWidth / (float)info.windowHeight, 0.1f, 1000.0f);

		// 라이팅 설정 ---------------------------------------
		float r = 2.f;
		vmath::vec3 lightPos = vmath::vec3((float)sin(animationTime * 0.5f) * r, 0.f, (float)cos(animationTime * 0.5f) * r);
		vmath::vec3 viewPos = cameraPos;

		vmath::vec3 lightAmbient(0.2f, 0.2f, 0.2f);
		vmath::vec3 lightDiffuse(0.5f, 0.5f, 0.5f);
		vmath::vec3 lightSpecular(1.0f, 1.0f, 1.0f);

		// 모델 그리기 ---------------------------------------
		glUseProgram(shader_program);

		glUniformMatrix4fv(glGetUniformLocation(shader_program, "projection"), 1, GL_FALSE, projM);
		glUniformMatrix4fv(glGetUniformLocation(shader_program, "view"), 1, GL_FALSE, lookAt);


		glUniform3fv(glGetUniformLocation(shader_program, "viewPos"), 1, viewPos);

		glUniform3fv(glGetUniformLocation(shader_program, "light.position"), 1, lightPos);
		glUniform3fv(glGetUniformLocation(shader_program, "light.ambient"), 1, lightAmbient);
		glUniform3fv(glGetUniformLocation(shader_program, "light.diffuse"), 1, lightDiffuse);
		glUniform3fv(glGetUniformLocation(shader_program, "light.specular"), 1, lightSpecular);

		// 빛효과 모든광원 쉐이더로 전달하기---------------------------
		//DirLight 광원
		glUniform3f(glGetUniformLocation(shader_program, "dirLight.direction"),0.3f, -1.0f, 0.5f);
		glUniform3f(glGetUniformLocation(shader_program,"dirLight.ambient"), 0.05f, 0.05f, 0.04f);
		glUniform3f(glGetUniformLocation(shader_program, "dirLight.diffuse"), 0.1f, 0.1f, 0.4f);
		glUniform3f(glGetUniformLocation(shader_program, "dirLight.specular"), 0.3f, 0.3f, 0.7f);

		//PointLight 광원
		glUniform3fv(glGetUniformLocation(shader_program, "pointLights[0].position"), 1, cameraPos);
		glUniform1f(glGetUniformLocation(shader_program, "pointLights[0].cutOff"), (float)cos(vmath::radians(12.5)));
		glUniform3f(glGetUniformLocation(shader_program, "pointLights[0].ambient"), 0.2f, 0.2f, 0.2f);
		glUniform3f(glGetUniformLocation(shader_program, "pointLights[0].diffuse"), 0.0f, 0.0f, 0.8f);
		glUniform3f(glGetUniformLocation(shader_program, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(shader_program, "pointLights[0].c1"), 0.09f);
		glUniform1f(glGetUniformLocation(shader_program, "pointLights[0].c2"), 0.032f);

		//SpotLight 광원 cameraPos, cameraPos + cameraFront
		glUniform3fv(glGetUniformLocation(shader_program, "spotLight.position"), 1, cameraPos);
		glUniform3fv(glGetUniformLocation(shader_program, "spotLight.direction"), 1, cameraFront);
		glUniform1f(glGetUniformLocation(shader_program, "spotLight.cutOff"), (float)cos(vmath::radians(5)));
		glUniform1f(glGetUniformLocation(shader_program, "spotLight.outerCutOff"), (float)cos(vmath::radians(20.5)));
		glUniform1f(glGetUniformLocation(shader_program, "spotLight.c1"), 0.09f);
		glUniform1f(glGetUniformLocation(shader_program, "spotLight.c2"), 0.032f);
		glUniform3f(glGetUniformLocation(shader_program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(shader_program, "spotLight.diffuse"), 0.4f, 0.4f, 0.4f);
		glUniform3f(glGetUniformLocation(shader_program, "spotLight.specular"), 0.6f, 0.6f, 0.6f);



		//카메라 위치 Check용!
		//cout<< cameraPos[0] << " "<<  cameraPos[2] << "\n";
		cout << cameraFront[0] << " "<< cameraFront[1] << " " << cameraFront[2] << "\n";

		if (lineMode)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		// Render-To-Texture Framebuffer Bind! ----------------------------------------
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glEnable(GL_DEPTH_TEST);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//houseModel 불러오기--------------------------
		if (drawModel) {
			glUniform1i(glGetUniformLocation(shader_program, "normalMap"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture[7]);
			vmath::mat4 model = vmath::translate(objPosition) *
				vmath::rotate(180.0f, 0.0f, 1.0f, 0.0f) *
				vmath::scale(0.6f);
			glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1, GL_FALSE, model);
			objModel.draw(shader_program);
		}

		//mushroom 불러오기
		if (drawModel) {
			glUniform1i(glGetUniformLocation(shader_program, "normalMap"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture[8]);
			vmath::mat4 model2 = vmath::translate(0.0f,-2.f,0.0f) *
				vmath::rotate(150.0f, 0.0f, 1.0f, 0.0f) *
				vmath::scale(9.f);
			glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1, GL_FALSE, model2);
			cuteModel.draw(shader_program);
		}

		//squidgirl 불러오기
		if (drawModel) {
			glUniform1i(glGetUniformLocation(shader_program, "normalMap"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture[9]);
			vmath::mat4 model3 = vmath::translate(237.0f, 0.f, -21.0f) *
				vmath::rotate(140.0f, 0.0f, 1.0f, 0.0f) *
				vmath::scale(0.2f);
			glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1, GL_FALSE, model3);
			squidgirlModel.draw(shader_program);
		}

		// 피라미드 그리기 ---------------------------------------
		if (drawLight) {
			vmath::mat4 transform = vmath::translate(cameraPos) * vmath::scale(1.5f);
			glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1, GL_FALSE, transform);
			pyramidModel.draw(shader_program);
		}

		//star 배경 그리기 
		glUseProgram(shader_program2);

		//shader_program 2도 유니폼 넘겨주기

		glUniformMatrix4fv(glGetUniformLocation(shader_program2, "projection2"), 1, GL_FALSE, projM);
		glUniformMatrix4fv(glGetUniformLocation(shader_program2, "view2"), 1, GL_FALSE, lookAt);

		if (drawLight) {
			glBindVertexArray(VAO[0]);
			glUniform1i(glGetUniformLocation(shader_program2, "texture1"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture[0]);
			vmath::mat4 starBackground = vmath::translate(vmath::vec3(100.0f,0.0f,60.0f)) * vmath::scale(2500.0f);
			glUniformMatrix4fv(glGetUniformLocation(shader_program2, "model2"), 1, GL_FALSE, starBackground);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
		//설명서 ----------------------------------------------
		if (drawLight) {
			glBindVertexArray(VAO[1]);
			glUniform1i(glGetUniformLocation(shader_program2, "texture1"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture[1]);
			vmath::mat4 manualTransform = vmath::translate(vmath::vec3(-15.0f, 0.0f, -65.0f))
										  *vmath::rotate(35.0f, 0.0f, 1.0f, 0.0f)
										  *vmath::scale(35.0f);
			glUniformMatrix4fv(glGetUniformLocation(shader_program2, "model2"), 1, GL_FALSE, manualTransform);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}

		//숫자2 --------------------------------------
		if (drawLight) {
			glBindVertexArray(VAO[2]);
			glUniform1i(glGetUniformLocation(shader_program2, "texture1"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture[2]);
			vmath::mat4 number2Transform = vmath::translate(vmath::vec3(-47.0f, -5.0f, 53.0f))
				* vmath::rotate(90.0f, 1.0f, 0.0f, 0.0f)
				* vmath::scale(8.0f);
			glUniformMatrix4fv(glGetUniformLocation(shader_program2, "model2"), 1, GL_FALSE, number2Transform);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}

		//숫자5 -------------------------------------------
		if (drawLight) {
			glBindVertexArray(VAO[3]);
			glUniform1i(glGetUniformLocation(shader_program2, "texture1"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture[3]);
			vmath::mat4 number3Transform = vmath::translate(vmath::vec3(276.0f, 100.0f, 322.0f))
				* vmath::rotate(-90.0f, 1.0f, 0.0f, 0.0f)
				* vmath::scale(17.0f);
			glUniformMatrix4fv(glGetUniformLocation(shader_program2, "model2"), 1, GL_FALSE, number3Transform);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}

		//숫자7 카드 ---------------------
		if (drawLight) {
			glBindVertexArray(VAO[4]);
			glUniform1i(glGetUniformLocation(shader_program2, "texture1"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture[4]);
			vmath::mat4 number4Transform = vmath::translate(vmath::vec3(264.0f, -10.0f, -33.0f))
				* vmath::rotate(90.0f, 1.0f, 0.0f, 0.0f)
				* vmath::scale(17.0f);
			glUniformMatrix4fv(glGetUniformLocation(shader_program2, "model2"), 1, GL_FALSE, number4Transform);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
		// 도어락-------------------------
		if (drawLight) {
			glBindVertexArray(VAO[5]);
			glUniform1i(glGetUniformLocation(shader_program2, "texture1"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture[5]);
			vmath::mat4 number5Transform = vmath::translate(vmath::vec3(97.0f, -100.0f, 97.0f))
				* vmath::rotate(0.0f, 0.0f, 0.0f, 0.0f)
				* vmath::scale(17.0f);
			glUniformMatrix4fv(glGetUniformLocation(shader_program2, "model2"), 1, GL_FALSE, number5Transform);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}

		//엔딩 장면 ---------------------------

		if (drawLight) {
			glBindVertexArray(VAO[6]);
			glUniform1i(glGetUniformLocation(shader_program2, "texture1"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture[6]);
			vmath::mat4 number6Transform = vmath::translate(vmath::vec3(97.0f, -150.0f, 97.0f))
				* vmath::rotate(0.0f, 0.0f, 0.0f, 0.0f)
				* vmath::scale(17.0f);
			glUniformMatrix4fv(glGetUniformLocation(shader_program2, "model2"), 1, GL_FALSE, number6Transform);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}


		// 윈도우 Framebuffer Bind! ----------------------------------------
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);

		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glUseProgram(shader_program_screen);
		glUniform1i(glGetUniformLocation(shader_program_screen, "screenTexture"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, FBO_texture);
		glBindVertexArray(VAO_screen);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		if (!onceCl) {
			initValues();
			onceCl = true;
		}

		


	}

	virtual void onResize(int w, int h)
	{
		sb7::application::onResize(w, h);

		if (glViewport != NULL)
			glViewport(0, 0, info.windowWidth, info.windowHeight);
	}

	virtual void init()
	{
		sb7::application::init();

		info.samples = 8;
		info.flags.debug = 1;
	}

	virtual void onKey(int key, int action)
	{
		if (action == GLFW_PRESS) {
			keys[key] = true; // 키가 눌리면 true로 설정
		}
		else if (action == GLFW_RELEASE) {
			keys[key] = false; // 키가 떼지면 false로 설정
		}
		if (action == GLFW_PRESS) {
			switch (key) {
			case ' ': // 스페이스바
				if (cameraPos[0] > 78.0f && cameraPos[0] < 118.0f && cameraPos[2] > 52.0f && cameraPos[2] < 124.0f) {
					doorlockOn = true;	
				}
				break;
			case 'M':
				PlaySound(NULL,0,0);
				break;
			case '1':
				drawModel = !drawModel;
				break;
			case '2':
				drawLight = !drawLight;
				break;
			case 'R':
				initValues();
				break;
			case '0': // 포스트 프로세싱 커널들을 위한 설정-----
				for (int i = 0; i < 9; i++) { // 기본
					kernelChoose[i] = 1.0f/5.0f;
				}
				glUniform1fv (glGetUniformLocation(shader_program_screen, "kernelChoose"),9,kernelChoose);
				break;
			case '9':
				for (int i = 0; i < 4; i++) { // 엣지
					kernelChoose[i] = 1.0f;
				}
				kernelChoose[4] = -9.0f;
				for (int i = 5; i < 9; i++) {
					kernelChoose[i] = 1.0f;
				}
				glUniform1fv(glGetUniformLocation(shader_program_screen, "kernelChoose"), 9, kernelChoose);
				break;
			case '8':
				kernelChoose[0] = 1.0f/16.0f;  // 블러
				kernelChoose[2] = 1.0f/16.0f;
				kernelChoose[6] = 1.0f/16.0f;
				kernelChoose[8] = 1.0f / 16.0f;
				for (int i = 1; i < 9; i+=2) {
					kernelChoose[i] = 2.0f/16.f;
				}
				kernelChoose[4] = 4.0f / 16.0f;
				
				glUniform1fv(glGetUniformLocation(shader_program_screen, "kernelChoose"), 9, kernelChoose);
			
				break;
			case '7':
				for (int i = 0; i < 4; i++) { // 샤프
					kernelChoose[i] = -1.0f;
				}
				kernelChoose[4] = 9.0f;
				for (int i = 5; i < 9; i++) {
					kernelChoose[i] = -1.0f;
				}
				glUniform1fv(glGetUniformLocation(shader_program_screen, "kernelChoose"), 9, kernelChoose);
				
				break;
			};
		}
	}

	void onMouseButton(int button, int action) {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			mouseDown = true;

			int x, y;
			getMousePosition(x, y);
			cout << x << " " << y << "\n";
			if (x >= 64 && x <= 265 && y >= 83 && y <= 406) {
				cout << "1\n";
				handleButtonPress(1); // 1 버튼 눌림
			}
			if (x >= 314 && x <= 513 && y >= 80 && y <= 230) {
				cout << "2\n";
				handleButtonPress(2); // 2 버튼 눌림
			}
			if (x >= 552 && x <= 737 && y >= 80 && y <= 221) {
				cout << "3\n";
				handleButtonPress(3); // 3 버튼 눌림
			}
			if (x >= 70 && x <= 266 && y >= 262 && y <= 406) {
				cout << "4\n";
				handleButtonPress(4); // 4 버튼 눌림
			}
			if (x >= 306 && x <= 513 && y >= 259 && y <= 408) {
				cout << "5\n";
				handleButtonPress(5); // 5 버튼 눌림
			}
			if (x >= 542 && x <= 745 && y >= 267 && y <= 411) {
				cout << "6\n";
				handleButtonPress(6); // 6 버튼 눌림
			}
			if (x >= 68 && x <= 269 && y >= 445 && y <= 585) {
				cout << "7\n";
				handleButtonPress(7); // 7 버튼 눌림
			}
			if (x >= 307 && x <= 513 && y >= 442 && y <= 581) {
				cout << "8\n";
				handleButtonPress(8); // 8 버튼 눌림
			}
			if (x >= 551 && x <= 738 && y >= 446 && y <= 583) {
				cout << "9\n";
				handleButtonPress(9); // 9 버튼 눌림
			}
		}
		else {
			mouseDown = false;
		}
	}

	void handleButtonPress(int value) {
		password[index] = value;
		index++;
		if (index == 3) {
			bool passwordTrue = true;
			for (int i = 0; i < 3; i++) {
				if (password[i] != correctPassword[i]) {
					passwordTrue = false;
					doorlockOn = false;
					cameraPos[1] = 0; //원 위치로
					break;  // 안 맞으면 바로 False.
				}
			}
			if (passwordTrue) {
				cout << "엔딩 위치로\n";
				Ending = true;
				
			}
			index = 0;  // 비밀번호 자리 확인한 뒤 배열 순서 초기화시키기.
		}
	}

	//virtual void onMouseMove(int x, int y)
	//{
	//	
	//	objYangle += float(x - mousePostion[0]) / 2.f;
	//	mousePostion = vmath::vec2(float(x), float(y));
	//}

	virtual void onMouseMove(int x, int y)
	{
		// 처음 초기화
		if (firstMouse) {
			lastX = x;
			lastY = y;
			firstMouse = false;
		}

		float xoffset = x - lastX;
		float yoffset = lastY - y; // y축은 상하 방향이 반대이니까
		lastX = x;
		lastY = y;

		// 최적의 값: 0.45f;
		float sensitivity = 0.45f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw += xoffset;
		pitch += yoffset;

		// pitch 값을 제한하기
		if (pitch > 55.0f)
			pitch = 55.0f;
		if (pitch < -55.0f)
			pitch = -55.0f;

		vmath::vec3 front;
		front[0] = cos(vmath::radians(yaw)) * cos(vmath::radians(pitch));
		front[1] = sin(vmath::radians(pitch));
		front[2] = sin(vmath::radians(yaw)) * cos(vmath::radians(pitch));
		cameraFront = vmath::normalize(front);
	}


#define MAX_FOV 120.f
#define MIN_FOV 10.f
	virtual void onMouseWheel(int pos)
	{
		
		if (pos > 0)
			fov = vmath::min(MAX_FOV, fov + 1.0f);
		else
			fov = vmath::max(MIN_FOV, fov - 1.0f);
	}

	void initValues()
	{
		drawModel = true;
		drawLight = true;
		pause = false;
		animationTime = 0;
		previousTime = 0;
		lineMode = false;

		mouseDown = false;

		fov = 50.f;

		objPosition = vmath::vec3(0.0f, -50.0f, 0.0f);
		objYangle = 0.f;

		cameraPos = vmath::vec3(-22.0f, 0.0f, -75.0f);
		cameraFront = vmath::vec3(0.574977f, 0.00959913f, 0.818113f);
		cameraUp = vmath::vec3(0.0f, 1.0f, 0.0f);
		cameraSpeed = 0.1f;

		yaw = -90.0f;
		pitch =90.0f;

		for (int i = 0; i < 9; i++) { // 기본 포스트 프로세싱을 위해 설정
			kernelChoose[i] = 1.0f / 5.0f;
		}
		glUniform1fv(glGetUniformLocation(shader_program_screen, "kernelChoose"), 9, kernelChoose);

		doorlockOn = false;
		passwordTrue = false;
		Ending = false;
		index = 0;

		//음악 재개
		PlaySound(TEXT("Night.wav"), NULL, SND_ASYNC || SND_LOOP);
		
	}

	//W, A, S, D로 키보드로 움직이기
	void updateCamera()
	{
		if (keys[GLFW_KEY_W]) {
			cameraPos += cameraSpeed * cameraFront;
		}
		if (keys[GLFW_KEY_S]) {
			cameraPos -= cameraSpeed * cameraFront;
		}
		if (keys[GLFW_KEY_A]) {
			cameraPos -= vmath::normalize(vmath::cross(cameraFront, cameraUp)) * cameraSpeed;
		}
		if (keys[GLFW_KEY_D]) {
			cameraPos += vmath::normalize(vmath::cross(cameraFront, cameraUp)) * cameraSpeed;
		}
		//달리기 키
		if (keys[GLFW_KEY_LEFT_SHIFT]) {
			cameraSpeed = 0.45f;
		}
		else {
			cameraSpeed = 0.17f;
		}
	}
	void restrictedArea(vmath::vec3* cameraPosition)
	{
		//외각바깥쪽
		if ((*cameraPosition)[0] > 343)
			(*cameraPosition)[0] = 343;
		if ((*cameraPosition)[0] < -180)
			(*cameraPosition)[0] = -180;
		if ((*cameraPosition)[2] < -105)
			(*cameraPosition)[2] = -105;
		if ((*cameraPosition)[2] > 457)
			(*cameraPosition)[2] = 457;


		int lastX = (*cameraPosition)[0];
		int lastZ = (*cameraPosition)[2];

		// 집 영역 제한
		if ((*cameraPosition)[0] > 10.0f && (*cameraPosition)[0] < 185.0f && (*cameraPosition)[2] > 133.0f && (*cameraPosition)[2] < 290.0f)
		{
			// 카메라 위치를 원래 위치로 되돌림
			(*cameraPosition)[0] = lastX;
			(*cameraPosition)[2] = lastZ;
		}
	}

	void doorlockCheck(vmath::vec3* cameraPosition, vmath::vec3* cameraFront) {
		fov = 40.0f;
		//조건에 맞으면 도어락보여줌
		if (Ending == false && doorlockOn == true) {
			(*cameraPosition)[0] = 97.0136f;
			(*cameraPosition)[1] = -100.0f;
			(*cameraPosition)[2] = 89.6511f;
			(*cameraFront)[0] = -0.00785418f;
			(*cameraFront)[1] = -0.00959888f;
			(*cameraFront)[2] = 0.999923f;
		}
		//조건에 맞으면 엔딩보여줌
		else if (Ending == true && doorlockOn == true)
		{
			(*cameraPosition)[0] = 97.0136f;
			(*cameraPosition)[1] = -150.0f;
			(*cameraPosition)[2] = 89.6511f;
			(*cameraFront)[0] = -0.00785418f;
			(*cameraFront)[1] = -0.00959888f;
			(*cameraFront)[2] = 0.999923f;
		}
	}

	
	

	



private:
	GLuint shader_program;
	GLuint shader_program2;


	Model objModel, pyramidModel,cuteModel, squidgirlModel;
	vmath::vec3 objPosition;
	float objYangle;

	bool drawModel, drawLight;
	bool lineMode;
	bool pause;
	double previousTime;
	double animationTime;

	vmath::vec2 mousePostion;
	bool mouseDown;

	float fov;

	vmath::vec3 cameraUp;
	vmath::vec3 cameraPos;
	vmath::vec3 cameraFront;
	float cameraSpeed;
	float yaw;
	float pitch;
	bool firstMouse;
	float lastX, lastY;

	bool keys[1024]; // 키 상태를 저장하기 위한 배열
    
	//private에서 특별히 선언 (반복문 안 쓰기 위함)
	int password[3] = { 0, 0, 0 }; // 비밀번호 배열 초기화 시킴
	int correctPassword[3] = { 2, 5, 7 }; //비밀번호 확인시키기 위한 배열
	int index = 0; //배열 위치 알려주는 변수.


	//VAO 선언
	GLuint VAO[7];
	GLuint texture[10];

	//프레임 버퍼 

	GLuint FBO, FBO_texture;
	GLuint VAO_screen, VBO_screen;

	GLuint shader_program_screen;
	
	float kernelChoose[9];

	// 처음에 한 번 초기화 시켜줌.
	bool onceCl;
	// 비밀번호 창으로 이동.
	bool doorlockOn;
	// 비밀번호 bool 변수.
	bool passwordTrue;
	//엔딩을 보여주는 bool 변수
	bool Ending;

	
};

// DECLARE_MAIN의 하나뿐인 인스턴스
DECLARE_MAIN(my_application)