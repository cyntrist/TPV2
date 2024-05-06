// This file is part of the course TPV2@UCM - Samir Genaim

#include "Networking.h"

#include <iostream>
#include "../utils/Vector2D.h"
#include "Game.h"
#include "LittleWolf.h"
#include "netwrok_messages.h"
#include "../sdlutils/SDLNetUtils.h"

Networking::Networking() :
	sock_(), //
	socketSet_(), //
	p_(), //
	srvadd_(), //
	clientId_(), //
	masterId_()
{
}

Networking::~Networking()
{
}

bool Networking::init(const char* host, Uint16 port)
{
	if (SDLNet_ResolveHost(&srvadd_, host, port) < 0)
	{
		SDLNetUtils::print_SDLNet_error();
		return false;
	}

	sock_ = SDLNet_UDP_Open(0);
	if (!sock_)
	{
		SDLNetUtils::print_SDLNet_error();
		return false;
	}

	p_ = SDLNet_AllocPacket(512);
	if (!p_)
	{
		SDLNetUtils::print_SDLNet_error();
		return false;
	}

	socketSet_ = SDLNet_AllocSocketSet(1);
	SDLNet_UDP_AddSocket(socketSet_, sock_);

	Msg m0;
	MsgWithMasterId m1;

	// request to connect
	m0._type = _CONNECTION_REQUEST;
	SDLNetUtils::serializedSend(m0, p_, sock_, srvadd_);

	bool connected = false;
	// wait 3sec for the response, if we don't get we stop (since done will be 'true')
	if (SDLNet_CheckSockets(socketSet_, 3000) > 0)
	{
		if (SDLNet_SocketReady(sock_))
		{
			if (SDLNetUtils::deserializedReceive(m0, p_, sock_) > 0)
			{
				switch (m0._type)
				{
				case _CONNECTION_ACCEPTED:
					std::cout << "Message type:: CONNECTION ACCEPTED" << std::endl;
					m1.deserialize(p_->data);
					clientId_ = m1._client_id;
					masterId_ = m1._master_id;
					connected = true;
					break;
				case _CONNECTION_REJECTED:
					std::cout << "Message type: CONNECTION REJECTED" << std::endl;
					break;
				}
			}
		}
	}

	if (!connected)
	{
		std::cerr << "Failed to get a client id" << std::endl;
		return false;
	}

#ifdef _DEBUG
	std::cout << "Connected with id " << static_cast<int>(clientId_) << std::endl;
#endif

	return true;
}

bool Networking::disconnect()
{
	MsgWithId m;
	m._type = _DISCONNECTED;
	m._client_id = clientId_;
	return (SDLNetUtils::serializedSend(m, p_, sock_, srvadd_) > 0);
}

void Networking::update()
{
	Msg m0;
	MsgWithMasterId m1;
	PlayerStateMsg m2;
	ShootMsg m3;
	MsgWithId m4;
	PlayerInfoMsg m5;

	while (SDLNetUtils::deserializedReceive(m0, p_, sock_) > 0)
	{
		switch (m0._type)
		{
		case _NEW_CLIENT_CONNECTED:
			std::cout << "Message type: NEW CLIENT CONNECTED" << std::endl;
			m1.deserialize(p_->data);
			masterId_ = m1._master_id;
			handle_new_client(m1._client_id);
			break;

		case _DISCONNECTED:
			std::cout << "Message type: DISCONNECTED" << std::endl;
			m1.deserialize(p_->data);
			masterId_ = m1._master_id;
			handle_disconnet(m1._client_id);
			break;

		case _PLAYER_STATE:
			std::cout << "Message type: PLAYER STATE" << std::endl;
			m2.deserialize(p_->data);
			handle_player_state(m2);
			break;

		case _PLAYER_INFO:
			m5.deserialize(p_->data);
			handle_player_info(m5);
			break;

		case _SHOOT:
			std::cout << "Message type: SHOOT" << std::endl;
			m3.deserialize(p_->data);
			handle_shoot(m3);
			break;

		case _DEAD:
			std::cout << "Message type: DEAD" << std::endl;
			m4.deserialize(p_->data);
			handle_dead(m4);
			break;

		case _RESTART:
			std::cout << "Message type: RESTART" << std::endl;
			handle_restart();
			break;


		case _SYNC:
			{
				SyncMsg m;
				m.deserialize(p_->data);
				handle_sync(m);
				break;
			}
		default:
			break;
		}
	}
}

void Networking::handle_new_client(Uint8 id)
{
	if (id != clientId_)
		Game::instance()->get_little_wolf()->send_my_info();
}

void Networking::handle_disconnet(Uint8 id)
{
	Game::instance()->get_little_wolf()->disconnect_player(id); ///
}

void Networking::send_state(const Vector2D& pos, float w, float h, float rot)
{
	PlayerStateMsg m;
	m._type = _PLAYER_STATE;
	m._client_id = clientId_;
	m.x = pos.getX();
	m.y = pos.getY();
	m.w = w;
	m.h = h;
	m.rot = rot;
	SDLNetUtils::serializedSend(m, p_, sock_, srvadd_);
}

void Networking::handle_player_state(const PlayerStateMsg& m)
{
	if (m._client_id != clientId_)
	{
		//Game::instance()->get_fighters().update_player_state(m._client_id, m.x, m.y, m.w, m.h, m.rot);
	}
}

void Networking::send_shoot(Vector2D p, Vector2D v, int width, int height,
                            float r)
{
	ShootMsg m;
	m._type = _SHOOT;
	m._client_id = clientId_;
	m.x = p.getX();
	m.y = p.getY();
	m.vx = v.getX();
	m.vy = v.getY();
	m.w = width;
	m.h = height;
	m.rot = r;
	SDLNetUtils::serializedSend(m, p_, sock_, srvadd_);
}

void Networking::handle_shoot(const ShootMsg& m)
{
	//Game::instance()->get_bullets().shoot(Vector2D(m.x, m.y), Vector2D(m.vx, m.vy), m.w, m.h, m.rot);
}

void Networking::send_dead(uint8_t id)
{
	MsgWithId m;
	m._type = _DEAD;
	m._client_id = id;
	SDLNetUtils::serializedSend(m, p_, sock_, srvadd_);
}

void Networking::handle_dead(const MsgWithId& m)
{
	//Game::instance()->get_fighters().killPlayer(m._client_id);
}

void Networking::send_my_info(const Vector2D& pos, const Vector2D& velocity,
                              float speed, float acceleration, float theta, Uint8 state)
{
	PlayerInfoMsg m;
	m._type = _PLAYER_INFO;

	m._client_id = clientId_;
	m.posX = pos.getX();
	m.posY = pos.getY();
	m.velX = velocity.getX();
	m.velY = velocity.getY();
	m.speed = speed;
	m.acceleration = acceleration;
	m.theta = theta;
	m.state = state;

	SDLNetUtils::serializedSend(m, p_, sock_, srvadd_);
}

void Networking::handle_player_info(const PlayerInfoMsg& m)
{
	const auto state = static_cast<LittleWolf::PlayerState>(m.state);
	if (m._client_id != clientId_)
	{
		Game::instance()->get_little_wolf()->update_my_info(
			m._client_id,
			m.posX,
			m.posY,
			m.velX,
			m.velY,
			m.speed,
			m.acceleration,
			m.theta,
			state);
	}
}

void Networking::send_restart()
{
	Msg m;
	m._type = _RESTART;
	SDLNetUtils::serializedSend(m, p_, sock_, srvadd_);
}

void Networking::send_sync(uint8_t id, const Vector2D& pos)
{
	SyncMsg m;

	m._client_id = id;
	m._type = _SYNC;
	m.posX = pos.getX();
	m.posY = pos.getY();

	SDLNetUtils::serializedSend(m, p_, sock_, srvadd_);
}

void Networking::handle_restart()
{
	//Game::instance()->get_fighters().bringAllToLife();
}

void Networking::handle_sync(const SyncMsg& m)
{
	Game::instance()->get_little_wolf()->sync(m._client_id, Vector2D(m.posX, m.posY));
}
