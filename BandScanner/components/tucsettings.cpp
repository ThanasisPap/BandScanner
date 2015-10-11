#include "tucsettings.h"

TucSettings::TucSettings()
{
    mSettings = new QSettings(INIT_FILENAME, QSettings::IniFormat);

    /*
     * If settings does not contain a basic value use the default
     */
    setDefaultSettings();
}

/**
 * @brief Settings::setDefaultSettings Sets the default settings. In theory
 * this should be executed the first time that the program runs.
 */
void TucSettings::setDefaultSettings()
{

    mSettingId[band_start].default_val      = BAND_START_VAL;
    mSettingId[band_start].id_str           = BAND_START_STR;
    mSettingId[band_end].default_val      = BAND_END_VAL;
    mSettingId[band_end].id_str           = BAND_END_STR;
    mSettingId[initial_band].default_val      = INITIAL_BAND_VAL;
    mSettingId[initial_band].id_str           = INITIAL_BAND_STR;
    mSettingId[band_step].default_val      = BAND_STEP_VAL;
    mSettingId[band_step].id_str           = BAND_STEP_STR;
//    mSettingId[band_start]      = {BAND_START_STR, BAND_START_VAL};
//    mSettingId[band_end]        = {BAND_END_STR, BAND_END_VAL};
//    mSettingId[initial_band]    = {INITIAL_BAND_STR, INITIAL_BAND_VAL};
//    mSettingId[band_step]       = {BAND_STEP_STR, BAND_STEP_VAL};

//    /*
//     * By default no lens are included
//     */
    mSettingId[filter_name1].default_val      = FIXED_NAME1_VAL;
    mSettingId[filter_name1].id_str           = FIXED_NAME1_STR;
    mSettingId[filter_name2].default_val      = FIXED_NAME2_VAL;
    mSettingId[filter_name2].id_str           = FIXED_NAME2_STR;
    mSettingId[filter_name3].default_val      = FIXED_NAME3_VAL;
    mSettingId[filter_name3].id_str           = FIXED_NAME3_STR;
    mSettingId[filter_band1].default_val      = FIXED_BAND1_VAL;
    mSettingId[filter_band1].id_str           = FIXED_BAND1_STR;
    mSettingId[filter_band2].default_val      = FIXED_BAND2_VAL;
    mSettingId[filter_band2].id_str           = FIXED_BAND2_STR;
    mSettingId[filter_band3].default_val      = FIXED_BAND3_VAL;
    mSettingId[filter_band3].id_str           = FIXED_BAND3_STR;
//    mSettingId[filter_name1]     = {FIXED_NAME1_STR, FIXED_NAME1_VAL};
//    mSettingId[filter_name2]     = {FIXED_NAME2_STR, FIXED_NAME2_VAL};
//    mSettingId[filter_name3]     = {FIXED_NAME3_STR, FIXED_NAME3_VAL};
//    mSettingId[filter_band1]     = {FIXED_BAND1_STR, FIXED_BAND1_VAL};
//    mSettingId[filter_band2]     = {FIXED_BAND2_STR, FIXED_BAND2_VAL};
//    mSettingId[filter_band3]     = {FIXED_BAND3_STR, FIXED_BAND3_VAL};
}

TucSettings::~TucSettings()
{
    delete mSettings;
}

QString TucSettings::readSetting(setting_ids id)
{
    QString value;

    /*
     * Attempt to read in the settings. If any are not available then the value is set
     * to it's default
     */
    if((mSettings->contains(mSettingId[id].id_str)) == false)
    {
        /*
         * New Setting, write the default Value
         */
        writeSetting(id, mSettingId[id].default_val);
    }

    value = mSettings->value(mSettingId[id].id_str).toString();

    return value;
}

void TucSettings::writeSetting(setting_ids id, QString value)
{
    mSettings->setValue(mSettingId[id].id_str, value);

    mSettings->sync();
}

