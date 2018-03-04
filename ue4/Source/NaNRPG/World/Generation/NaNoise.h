// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <algorithm>

/**
 * �p�[�����m�C�Y�W�F�l���[�^
 */
class FNaNoise
{
public:
	
public:
	//! �R���X�g���N�^
	FNaNoise( int32 seed = 0 );

	//! 
	void reseed( int32 seed )
	{
		m_Random.Initialize( seed );

		for ( int32 i = 0; i < 256; ++i ){
			p[i] = i;
		}

		for ( int32 i = 0; i < 256; ++i ){
			int32	t,tmp;

			t		= m_Random.RandRange( 0, 255 );
			tmp		= p[i];
			p[i]	= p[t];
			p[t]	= tmp;
		}
//		std::shuffle( std::begin(p), std::begin(p) + 256, std::default_random_engine( seed ) );

		for ( int32 i = 0; i < 256; ++i ){
			p[256 + i] = p[i];
		}
	}

	//! 
	float noise(float x) const
	{
		return noise(x, 0.0f, 0.0f);
	}

	//! 
	float noise(float x, float y) const
	{
		return noise(x, y, 0.0f);
	}

	//! 
	float noise(float x, float y, float z) const
	{
		const int32 X = FMath::FloorToInt(x) & 0xFF;
		const int32 Y = FMath::FloorToInt(y) & 0xFF;
		const int32 Z = FMath::FloorToInt(z) & 0xFF;

		x	-= FMath::FloorToFloat(x);
		y	-= FMath::FloorToFloat(y);
		z	-= FMath::FloorToFloat(z);

		const float u = fade(x);
		const float v = fade(y);
		const float w = fade(z);

		const int A = p[X] + Y, AA = p[A] + Z, AB = p[A + 1] + Z;
		const int B = p[X + 1] + Y, BA = p[B] + Z, BB = p[B + 1] + Z;

		return lerp(w, lerp(v, lerp(u, grad(p[AA], x, y, z),
			grad(p[BA], x - 1, y, z)),
			lerp(u, grad(p[AB], x, y - 1, z),
			grad(p[BB], x - 1, y - 1, z))),
			lerp(v, lerp(u, grad(p[AA + 1], x, y, z - 1),
			grad(p[BA + 1], x - 1, y, z - 1)),
			lerp(u, grad(p[AB + 1], x, y - 1, z - 1),
			grad(p[BB + 1], x - 1, y - 1, z - 1))));
	}

	float octaveNoise( float x, int32 octaves ) const
	{
		float result = 0.0f;
		float amp = 1.0f;

		for ( int32 i = 0; i < octaves; ++i ){
			result += noise(x) * amp;
			x *= 2.0f;
			amp *= 0.5f;
		}

		return result;
	}

	float octaveNoise( float x, float y, int32 octaves ) const
	{
		float result = 0.0f;
		float amp = 1.0f;

		for ( int32 i = 0; i < octaves; ++i){
			result += noise(x, y) * amp;
			x *= 2.0f;
			y *= 2.0f;
			amp *= 0.5f;
		}

		return result;
	}

	float octaveNoise( float x, float y, float z, int32 octaves ) const
	{
		float result = 0.0f;
		float amp = 1.0f;

		for ( int32 i = 0; i < octaves; ++i ){
			result += noise(x, y, z) * amp;
			x *= 2.0f;
			y *= 2.0f;
			z *= 2.0f;
			amp *= 0.5f;
		}

		return result;
	}

	float noise0_1(float x) const
	{
		return noise(x) * 0.5f + 0.5f;
	}

	float noise0_1(float x, float y) const
	{
		return noise(x, y) * 0.5f + 0.5f;
	}

	float noise0_1(float x, float y, float z) const
	{
		return noise(x, y, z) * 0.5f + 0.5f;
	}

	float octaveNoise0_1(float x, int32 octaves) const
	{
		return octaveNoise(x, octaves) * 0.5f + 0.5f;
	}

	float octaveNoise0_1(float x, float y, int32 octaves) const
	{
		return octaveNoise(x, y, octaves) * 0.5f + 0.5f;
	}

	float octaveNoise0_1(float x, float y, float z, int32 octaves) const
	{
		return octaveNoise(x, y, z, octaves) * 0.5f + 0.5f;
	}

protected:
	//! 
	float fade( float t ) const
	{
		return t * t * t * (t * (t * 6 - 15) + 10);
	}

	//! 
	float lerp( float t, float a, float b ) const
	{
		return a + t * (b - a);
	}

	//! 
	float grad( int32 hash, float x, float y, float z ) const
	{
		const int32 h = hash & 15;
		const float u = h < 8 ? x : y;
		const float v = h < 4 ? y : h == 12 || h == 14 ? x : z;

		return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
	}

public:
	
protected:
	//! 
	FRandomStream	m_Random;
	//! 
	int32			p[512];

};

/*
namespace siv
{
	class PerlinNoise
	{
	private:

		std::int32_t p[512];

		double fade(double t) const noexcept
		{
			return t * t * t * (t * (t * 6 - 15) + 10);
		}

		double lerp(double t, double a, double b) const noexcept
		{
			return a + t * (b - a);
		}

		double grad(std::int32_t hash, double x, double y, double z) const noexcept
		{
			const std::int32_t h = hash & 15;
			const double u = h < 8 ? x : y;
			const double v = h < 4 ? y : h == 12 || h == 14 ? x : z;
			return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
		}

	public:

		explicit PerlinNoise(std::uint32_t seed = std::default_random_engine::default_seed)
		{
			reseed(seed);
		}

		void reseed(std::uint32_t seed)
		{
			for (std::int32_t i = 0; i < 256; ++i)
			{
				p[i] = i;
			}

			std::shuffle(std::begin(p), std::begin(p) + 256, std::default_random_engine(seed));

			for (size_t i = 0; i < 256; ++i)
			{
				p[256 + i] = p[i];
			}
		}

		double noise(double x) const
		{
			return noise(x, 0.0, 0.0);
		}

		double noise(double x, double y) const
		{
			return noise(x, y, 0.0);
		}

		double noise(double x, double y, double z) const
		{
			const std::int32_t X = static_cast<std::int32_t>(std::floor(x)) & 255;
			const std::int32_t Y = static_cast<std::int32_t>(std::floor(y)) & 255;
			const std::int32_t Z = static_cast<std::int32_t>(std::floor(z)) & 255;

			x -= std::floor(x);
			y -= std::floor(y);
			z -= std::floor(z);

			const double u = fade(x);
			const double v = fade(y);
			const double w = fade(z);

			const int A = p[X] + Y, AA = p[A] + Z, AB = p[A + 1] + Z;
			const int B = p[X + 1] + Y, BA = p[B] + Z, BB = p[B + 1] + Z;

			return lerp(w, lerp(v, lerp(u, grad(p[AA], x, y, z),
				grad(p[BA], x - 1, y, z)),
				lerp(u, grad(p[AB], x, y - 1, z),
				grad(p[BB], x - 1, y - 1, z))),
				lerp(v, lerp(u, grad(p[AA + 1], x, y, z - 1),
				grad(p[BA + 1], x - 1, y, z - 1)),
				lerp(u, grad(p[AB + 1], x, y - 1, z - 1),
				grad(p[BB + 1], x - 1, y - 1, z - 1))));
		}

		double octaveNoise(double x, std::int32_t octaves) const
		{
			double result = 0.0;
			double amp = 1.0;

			for (std::int32_t i = 0; i < octaves; ++i)
			{
				result += noise(x) * amp;
				x *= 2.0;
				amp *= 0.5;
			}

			return result;
		}

		double octaveNoise(double x, double y, std::int32_t octaves) const
		{
			double result = 0.0;
			double amp = 1.0;

			for (std::int32_t i = 0; i < octaves; ++i)
			{
				result += noise(x, y) * amp;
				x *= 2.0;
				y *= 2.0;
				amp *= 0.5;
			}

			return result;
		}

		double octaveNoise(double x, double y, double z, std::int32_t octaves) const
		{
			double result = 0.0;
			double amp = 1.0;

			for (std::int32_t i = 0; i < octaves; ++i)
			{
				result += noise(x, y, z) * amp;
				x *= 2.0;
				y *= 2.0;
				z *= 2.0;
				amp *= 0.5;
			}

			return result;
		}

		double noise0_1(double x) const
		{
			return noise(x) * 0.5 + 0.5;
		}

		double noise0_1(double x, double y) const
		{
			return noise(x, y) * 0.5 + 0.5;
		}

		double noise0_1(double x, double y, double z) const
		{
			return noise(x, y, z) * 0.5 + 0.5;
		}

		double octaveNoise0_1(double x, std::int32_t octaves) const
		{
			return octaveNoise(x, octaves) * 0.5 + 0.5;
		}

		double octaveNoise0_1(double x, double y, std::int32_t octaves) const
		{
			return octaveNoise(x, y, octaves) * 0.5 + 0.5;
		}

		double octaveNoise0_1(double x, double y, double z, std::int32_t octaves) const
		{
			return octaveNoise(x, y, z, octaves) * 0.5 + 0.5;
		}
	};
}
*/