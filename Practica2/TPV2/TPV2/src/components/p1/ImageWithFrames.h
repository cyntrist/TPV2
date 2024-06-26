﻿#pragma once
#include "../Transform.h"
#include "../../ecs/Component.h"
#include "../../sdlutils/Texture.h"

struct ImageWithFrames : ecs::Component
{
	__CMPID_DECL__(ecs::cmp::IMAGEWITHFRAMES)

	Texture* image_;
	Transform* transform_;

	// 85 x 100
	int currentRow_; // fila actual
	int currentCol_; // columna actual

	int frameWidth_; // anchura de los frames
	int frameHeight_; // altura de los frames

	int nRows_; // número de filas
	int nCols_; // número de columnas

	int firstFrame; // indice de la primera frame de la animacion
	int lastFrame; // indice de la ultima frame de la animacion
	int currentFrame; // indice del frame actual

	Uint32 frameTimer; // contador del tiempo

	ImageWithFrames() = default;
	ImageWithFrames(Texture*, int, int, int, int);
	~ImageWithFrames() override;
	void initComponent() override;
	void render() override;
};
