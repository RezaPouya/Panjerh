
struct FragmentShader {
public:
	const char* DefaultVertextShader() {
		return R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        void main()
        {
            gl_Position = vec4(aPos.x , aPos.y , aPos.z, 1.0);
        })";
	}

    const char* DefaultFraShader() {
        return R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        void main()
        {
            gl_Position = vec4(aPos.x , aPos.y , aPos.z, 1.0);
        })";
    }

    const char* DefaultFragmentShader() {
        return R"(
        #version 330 core
        in vec3 aPos;
        in vec3 aColor;
        out vec3 vertexColor;
        void main()
        {
            gl_Position = vec4(aPos, 1.0);
            vertexColor = aColor;
        }
    )";
    }
};