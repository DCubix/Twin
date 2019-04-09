#ifndef TWIN_HPP
#define TWIN_HPP

#include <cmath>
#include <vector>
#include <initializer_list>
#include <functional>
#include <algorithm>

#define TWIN_PENNER 1.70158f
#define TWIN_PI 3.141592654f
#define TWIN_EPSILON 1e-5f
#define TWIN_LERP(a, b, t) ((1.0f - t) * a + b * t)

namespace twin {
	namespace funcs {
		inline float Linear(float t) { return t; }
		inline float InQuad(float t) { return t * t; }
		inline float OutQuad(float t) { return t * (2.0f - t); }
		inline float InOutQuad(float t) {
			return t < 0.5f ? 2.0f * t * t : -1.0f + (4.0f - 2.0f * t) * t;
		}
		inline float InCubic(float t) { return t * t * t; }
		inline float OutCubic(float t) { return (--t) * t * t + 1.0f; }
		inline float InOutCubic(float t) {
			float k = 2.0f * t - 2.0f;
			return t < 0.5f ? 4.0f * t * t * t : (t - 1.0f) * k * k + 1.0f;
		}
		inline float InQuart(float t) { return t * t * t * t; }
		inline float OutQuart(float t) { return 1.0f - (--t) * t * t * t; }
		inline float InOutQuart(float t) {
			return t < 0.5f ? 8.0f * t * t * t * t : 1.0f - 8.0f * (--t) * t * t * t;
		}
		inline float InQuint(float t) { return t * t * t * t * t; }
		inline float OutQuint(float t) { return 1.0f + (--t) * t * t * t * t; }
		inline float InOutQuint(float t) {
			return t < 0.5f ? 16.0f * t * t * t * t * t : 1.0f + 16.0f * (--t) * t * t * t * t;
		}
		inline float InSine(float t) { return -1.0f * ::cosf(t / 1.0f * (TWIN_PI * 0.5f)) + 1.0f;}
		inline float OutSine(float t) { return ::sinf(t / 1.0f * (TWIN_PI * 0.5f)); }
		inline float InOutSine(float t) { return -1.0f / 2.0f * (::cosf(TWIN_PI * t) - 1.0f); }
		inline float InExpo(float t) {
			return (t <= TWIN_EPSILON) ? 0.0f : ::powf(2.0f, 10.0f * (t - 1.0f));
		}
		inline float OutExpo(float t) {
			return (t >= 1.0f - TWIN_EPSILON) ? 1.0f : (-::powf(2.0f, -10.0f * t) + 1.0f);
		}
		inline float InOutExpo(float t) {
			if (t <= TWIN_EPSILON) return 0.0f;
			if (t >= 1.0f - TWIN_EPSILON) return 1.0f;
			if ((t /= 1.0f / 2.0f) < 1.0f) return 1.0f / 2.0f * ::powf(2.0f, 10.0f * (t - 1.0f));
			return 1.0f / 2.0f * (-::powf(2.0f, -10.0f * --t) + 2.0f);
		}
		inline float InCirc(float t) {
			return -1.0f * (::sqrtf(1.0f - t * t) - 1.0f);
		}
		inline float OutCirc(float t) {
			return ::sqrtf(1.0f - (t -= 1.0f) * t);
		}
		inline float InOutCirc(float t) {
			if ((t /= 0.5f) < 1.0f) return -1.0f / 2.0f * (::sqrtf(1.0f - t * t) - 1.0f);
			return 1.0f / 2.0f * (::sqrtf(1.0f - (t -= 2.0f) * t) + 1.0f);
		}
		inline float InElastic(float t) {
			float s = TWIN_PENNER;
			float p = 0.0f, a = 1.0f;
			if (t <= TWIN_EPSILON) return 0.0f;
			if (t >= 1.0f - TWIN_EPSILON) return 1.0f;
			if (!p) p = 0.3f;
			if (a < 1.0f) {
				a = 1.0f;
				s = p / 4.0f;
			} else s = p / (2.0f * TWIN_PI) * ::asinf(1.0f / a);
			return -(a * ::powf(2.0f, 10.0f * (t -= 1.0f)) * ::sinf((t - s) * (2.0f * TWIN_PI) / p));
		}
		inline float OutElastic(float t) {
			float s = TWIN_PENNER;
			float p = 0.0f, a = 1.0f;
			if (t <= TWIN_EPSILON) return 0.0f;
			if (t >= 1.0f - TWIN_EPSILON) return 1.0f;
			if (!p) p = 0.3f;
			if (a < 1.0f) {
				a = 1.0f;
				s = p / 4.0f;
			} else s = p / (2.0f * TWIN_PI) * ::asinf(1.0f / a);
			return a * ::powf(2.0f, -10.0f * t) * ::sinf((t - s) * (2.0f * TWIN_PI) / p) + 1.0f;
		}
		inline float InOutElastic(float t) {
			float s = TWIN_PENNER;
			float p = 0;
			float a = 1;
			if (t <= TWIN_EPSILON) return 0.0f;
			if ((t /= 0.5f) >= 2.0f - TWIN_EPSILON) return 1.0f;
			if (!p) p = (0.3f * 1.5f);
			if (a < 1.0f) {
				a = 1.0f;
				s = p / 4.0f;
			} else s = p / (2.0f * TWIN_PI) * ::asinf(1.0f / a);
			if (t < 1.0f)
				return -0.5f * (a * ::powf(2.0f, 10.0f * (t -= 1.0f)) * ::sinf((t - s) * (2.0f * TWIN_PI) / p));
			return a * ::powf(2.0f, -10.0f * (t -= 1.0f)) * ::sinf((t - s) * (2.0f * TWIN_PI) / p) * 0.5f + 1.0f;
		}
		inline float InBack(float t) {
			return 1.0f * t * t * ((TWIN_PENNER + 1.0f) * t - TWIN_PENNER);
		}
		inline float OutBack(float t) {
			return 1.0f * ((t = t / 1.0f - 1.0f) * t * ((TWIN_PENNER + 1.0f) * t + TWIN_PENNER) + 1.0f);
		}
		inline float InOutBack(float t) {
			float s = TWIN_PENNER;
			if ((t /= 0.5f) < 1.0f) return 1.0f / 2.0f * (t * t * (((s *= (1.525f)) + 1.0f) * t - s));
			return 1.0f / 2.0f * ((t -= 2.0f) * t * (((s *= (1.525f)) + 1.0f) * t + s) + 2.0f);
		}
		inline float OutBounce(float t) {
			if ((t /= 1.0f) < (1.0f / 2.75f)) {
				return (7.5625f * t * t);
			} else if (t < (2.0f / 2.75f)) {
				return (7.5625f * (t -= (1.5f / 2.75f)) * t + 0.75f);
			} else if (t < (2.5f / 2.75f)) {
				return (7.5625f * (t -= (2.25f / 2.75f)) * t + 0.9375f);
			} else {
				return (7.5625f * (t -= (2.625f / 2.75f)) * t + 0.984375f);
			}
		}
		inline float InBounce(float t) {
			return 1.0f - OutBounce(1.0f - t);
		}
		inline float InOutBounce(float t) {
			if (t < 0.5f) return InBounce(t * 2.0f) * 0.5f;
			return OutBounce(t * 2.0f - 1.0f) * 0.5f + 0.5f;
		}
	}

	using Easing = std::function<float(float)>;

	class Twin {
		using ValuesPtr = std::vector<float*>;
		using Values = std::initializer_list<float>;
	public:
		inline Twin& key(const Values& values, Easing easing = funcs::Linear, float time = -1.0f) {
			Key k{};
			k.easing = easing;
			k.values = values;
			k.time = time;
			m_keys.push_back(k);
			return *this;
		}

		inline Twin& key(const Values& values, float time) {
			return key(values, funcs::Linear, time);
		}

		inline void update(float dt) {
			if (m_time >= 1.0f + TWIN_EPSILON) {
				if (!m_loop) return;
				else m_time = 0.0f;
			}
			if (m_keys.empty() || m_keys.size() < 2) return;

			const float tfac = 1.0f / (m_keys.size()-1);
			for (size_t i = 0; i < m_keys.size() - 1; i++) {
				float ta = m_keys[i].time > -1.0f ? m_keys[i].time : i * tfac;
				float tb = m_keys[i + 1].time > -1.0f ? m_keys[i + 1].time : (i + 1) * tfac;
				if (m_time >= ta && m_time <= tb) {
					float factor = (m_time - ta) / (tb - ta);
					float t = m_keys[i].easing(factor);
					for (size_t j = 0; j < m_values.size(); j++)
						*m_values[j] = TWIN_LERP(m_keys[i].values[j], m_keys[i + 1].values[j], t);
					break;
				}
			}
			m_time += dt / m_duration;
		}
		
		inline Twin() = default;
		inline ~Twin() = default;

		inline Twin(const ValuesPtr& values, float duration = 1.0f, bool loop = false) {
			m_time = 0.0f;
			m_values = values;
			m_duration = duration;
			m_loop = loop;
		}

	private:
		struct Key {
			float time;
			std::vector<float> values;
			Easing easing;
		};

		std::vector<Key> m_keys;
		ValuesPtr m_values;
		float m_time, m_duration;
		bool m_loop;
	};

}

#endif // TWIN_HPP