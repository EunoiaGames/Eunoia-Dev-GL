#include "Math.h"

namespace Eunoia {

	m4 m4::CreateTransformation(const v3& translation, const v3& scale, const quat& rot)
	{
		return CreateTranslation(translation) * (rot.CreateRotationMatrix() * CreateScale(scale));
	}

	m4 m4::CreateView(const v3& cameraPos, const quat& cameraRot)
	{
		return cameraRot.Conjugate().CreateRotationMatrix() * m4::CreateTranslation(cameraPos * -1.0f);
	}

	v3 v3::Rotate(const quat& rot) const
	{
		quat conj = rot.Conjugate();
		quat w = rot * (*this) * conj;
		return v3(w.x, w.y, w.z);
	}

}