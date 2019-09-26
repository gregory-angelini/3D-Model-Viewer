#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>


namespace glm
{
	enum
	{
		X = 0,  
		Y = 1,  
		Z = 2,  
		W = 3  
	};

	template<typename> class init
	{};

	template<> class init<glm::quat>
	{
		public: static glm::quat identity()
		{
			return glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		}
	};


	template<> class init<glm::mat4>
	{
		public: static glm::mat4 identity()
		{
			return glm::identity<glm::mat4>();
		}
	};

	template<> class init<glm::vec3>
	{
		public: static glm::vec3 unitX()
		{
			return glm::vec3(1.0f, 0.0f, 0.0f);
		}

		public: static glm::vec3 unitY()
		{
			return glm::vec3(0.0f, 1.0f, 0.0f);
		}

		public: static glm::vec3 unitZ()
		{
			return glm::vec3(0.0f, 0.0f, 1.0f);
		}
	};
};

