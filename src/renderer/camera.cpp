#include "camera.hpp"
#include "graphics_system.hpp"

Vec2f Camera::getPosition()const
{
	return service::renderer::ref().getCamera()->getPosition();
}

void Camera::setPosition(const Vec2f& position)
{
	service::renderer::ref().getCamera()->setPosition(position);
}

float Camera::getWidth()const
{
    return service::renderer::ref().getCamera()->getWidth();
}

float Camera::getHeight()const
{
    return service::renderer::ref().getCamera()->getHeight();
}

void Camera::setWidth(float width)
{
	service::renderer::ref().getCamera()->setWidth(width);
}

void Camera::setHeight(float height)
{
	service::renderer::ref().getCamera()->setHeight(height);
}
