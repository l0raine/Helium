#pragma once

#include "API.h"
#include "Control.h"

namespace Inspect
{
  const static char SLIDER_ATTR_MIN[] = "min";
  const static char SLIDER_ATTR_MAX[] = "max";

  class INSPECT_API Slider : public Reflect::ConcreteInheritor<Slider, Control>
  {
  private:
    float m_Min;
    float m_Max;
    float m_CurrentValue;
    float m_StartDragValue;
    bool m_Tracking;
    bool m_AutoAdjustMinMax;

  public:
    Slider();

    virtual void Realize( Container* parent );
    virtual void Read();
    virtual bool Write();
    virtual void Start();
    virtual void End();
    virtual float GetValue();
    virtual void SetValue( float value );
    virtual void SetRangeMin( float min, bool clamp = true );
    virtual void SetRangeMax( float max, bool clamp = true );
    virtual void SetAutoAdjustMinMax( bool autoAdjust );

  protected:
    virtual bool Process( const std::string& key, const std::string& value );
    void SetUIValue( float value );
    float GetUIValue() const;
  };

  typedef Nocturnal::SmartPtr<Slider> SliderPtr;
}