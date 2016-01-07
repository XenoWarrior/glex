#include "GameCamera.h"

/**
 * TODO
 */
GameCamera::GameCamera(glm::vec3 p)
{
	position = p;
}

/**
 * Changes block distance
 */
void GameCamera::ChangeBlockDist(int i)
{
	block_dist += i;

	// Check distance
	if(block_dist < 1)
	{
		block_dist = 1;
	}
	if(block_dist > 10)
	{
		block_dist = 10;
	}
}

/**
 * TODO
 */
glm::vec3 GameCamera::DrawCamera()
{
	// Resetting camera position X
	if(camera_x >= 6.280)
		camera_x = 0.1;
	if(camera_x <= 0)
		camera_x = 6.280;

	// Resetting camera position Y
	if(camera_y >= 1.5f)
		camera_y = 1.5;
	if(camera_y <= -1.5f)
		camera_y = -1.5;

	glm::vec3 direction(
		cos(camera_y) * sin(camera_x),
		sin(camera_y),
		cos(camera_y) * cos(camera_x)
	);

	glm::vec3 move_direction(
		cos(camera_y) * sin(camera_x),
		0,
		cos(camera_y) * cos(camera_x)
	);
	z_direction = move_direction;

	x_direction = glm::vec3(
		sin(camera_x - 3.14f/2.0f),
		0,
		cos(camera_x - 3.14f/2.0f)
	);

	glm::vec3 vup = glm::cross(x_direction, direction);
	glm::mat4 cam_proj = glm::perspective(75.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	cam_view = glm::lookAt(
		position,
		position + direction,
		vup
	);
	glm::mat4 cam_mod(1.0f);

	glUniformMatrix4fv(0, 1, GL_FALSE, &cam_proj[0][0]);
	glUniformMatrix4fv(1, 1, GL_FALSE, &cam_view[0][0]);
	glUniformMatrix4fv(2, 1, GL_FALSE, &cam_mod[0][0]);
}

/**
 * Returns an offset for the space in front of the camera
 * and also calculates block placement based on the camera angle
 */
glm::vec3 GameCamera::GetOffset()
{
	int x = 0, y = 0, z = 0, s = 0;
	float p = 0.0f;

	if(f_pos == "N")
	{
		if(camera_x < 1)
			p = camera_x;
		else
			p = camera_x - point*16;

		s = int(floor(p * 10));
		z += block_dist;
		x += s;
	}
	if(f_pos == "E")
	{
		s = int(floor((point*12 - camera_x) * 10));
		x -= block_dist;
		z -= s;	
	}
	if(f_pos == "S")
	{
		s = int(floor((point*8 - camera_x) * 10));
		z -= block_dist;
		x += s;
	}
	if(f_pos == "W")
	{
		s = int(floor((point*4 - camera_x) * 10));
		x += block_dist;
		z += s;
	}

	if(f_pos == "NE")
	{
		z += block_dist;
		x -= block_dist;
	}
	if(f_pos == "SE")
	{
		z -= block_dist;
		x -= block_dist;
	}
	if(f_pos == "SW")
	{
		z -= block_dist;
		x += block_dist;
	}
	if(f_pos == "NW")
	{
		z += block_dist;
		x += block_dist;
	}

	s = int(camera_y * 10);
	if(s > block_dist)
	{
		s = block_dist;
	}
	if(s < (0 - block_dist))
	{
		s = (0 - block_dist);
	}
	y += s;

	if(x > block_dist)
		x = block_dist;
	if(y > block_dist)
		y = block_dist;
	if(z > block_dist)
		z = block_dist;

	if(x < 0 - block_dist)
		x = 0 - block_dist;
	if(y < 0 - block_dist)
		y = 0 - block_dist;
	if(z < 0 - block_dist)
		z = 0 - block_dist;

	return glm::vec3(x, y, z);
}

/**
* Handles the keyboard inputs passed from main.
*/
void GameCamera::CameraController(int k)
{
	/* Slow collision checker
		std::vector<std::shared_ptr<CubeAsset>> asset_list = asset_manager->GetAssets();
		for(int i = 0; i < asset_list.size(); i++)
		{
			CubeAsset c = *asset_list[i];
			if(CheckCollision(c.GetVec3()))
			{
				std::cout << "[P]Detected collision with cube at: (X: " << position.x << ", Y: " << position.y << ", Z: " << position.z << ")" << std::endl;
				std::cout << "[C]Detected collision with cube at: (X: " << c.GetVec3().x << ", Y: " << c.GetVec3().y << ", Z: " << c.GetVec3().z << ")" << std::endl;
			}
		}
	*/

	// For W A S D
	if(k == 1)
		position += z_direction * camera_speed;
	if(k == 2)
		position -= x_direction * camera_speed;
	if(k == 3)
		position -= z_direction * camera_speed;
	if(k == 4)
		position += x_direction * camera_speed;

	// I K 
	if(k == 5)
		camera_y += 0.5f * camera_speed;
	if(k == 7)
		camera_y -= 0.5f * camera_speed;

	// J L
	if(k == 6)
		camera_x += 0.5f * camera_speed;
	if(k == 8)
		camera_x -= 0.5f * camera_speed;

	// For R F
	if(k == 9)
		position.y += 0.5f * camera_speed;
	if(k == 10)
		position.y -= 0.5f * camera_speed;

}

/**
 * Handles the mouse input for the camera
 */
void GameCamera::MoveCamera(glm::vec2 motion, glm::vec2 display)
{
	camera_x += ((display.x/2) - motion.x) * camera_speed / 100.0f;
	camera_y += ((display.y/2) - motion.y) * camera_speed / 100.0f;
}

/**
 * Calculates the camera facing direction
 */
void GameCamera::UpdateFacingDirection()
{
	if(camera_x <= point && camera_x >= 0.0 && f_pos != "N")
	{
		f_pos = "N";
		return;
	}
	if(camera_x >= point && camera_x <= point*3 && f_pos != "NW")
	{
		f_pos = "NW";
		return;
	}
	if(camera_x >= point*3 && camera_x <= point*5 && f_pos != "W")
	{
		f_pos = "W";
		return;
	}
	if(camera_x >= point*5 && camera_x <= point*7 && f_pos != "SW")
	{
		f_pos = "SW";
		return;
	}
	if(camera_x >= point*7 && camera_x <= point*9 && f_pos != "S")
	{
		f_pos = "S";
		return;
	}
	if(camera_x >= point*9 && camera_x <= point*11 && f_pos != "SE")
	{
		f_pos = "SE";
		return;
	}
	if(camera_x >= point*11 && camera_x <= point*13 && f_pos != "E")
	{
		f_pos = "E";
		return;
	}
	if(camera_x >= point*13 && camera_x <= point*15 && f_pos != "NE")
	{
		f_pos = "NE";
		return;
	}
	if(camera_x >= point*15 && camera_x <= point*17 && f_pos != "N")
	{
		f_pos = "N";
		return;
	}
}