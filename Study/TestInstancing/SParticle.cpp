#include "SParticle.h"

SParticle::SParticle()
{
	int iRand	= rand();
	m_vPos = Vector3(0.0f, 0.0f, 0.0f);
	m_fLifeTime = (float)(rand() % 5) + 5.0f;
	m_fTimer = 0.0f;
	m_vAdd = Vector3(cosf(iRand), sinf(iRand), 0.0f);
	m_vColor.x = (float)(rand() % RAND_MAX) / RAND_MAX;
	m_vColor.y = (float)(rand() % RAND_MAX) / RAND_MAX;
	m_vColor.z = (float)(rand() % RAND_MAX) / RAND_MAX;
	m_vColor.w = 1.0f;
}
SParticle::~SParticle()
{

}