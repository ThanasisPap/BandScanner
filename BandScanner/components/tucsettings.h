#ifndef TUCSETTINGS_H
#define TUCSETTINGS_H

#include <QString>
#include <QSettings>

#define ORGANIZATION_NAME       "TUC"
#define APPLICATION_NAME        "BandScanner"
#define INIT_FILENAME           "bandscanner.ini"

typedef enum {
    band_start,
    band_end,
    initial_band,
    band_step,
    filter_name1,
    filter_band1,
    filter_name2,
    filter_band2,
    filter_name3,
    filter_band3
}setting_ids;

#define BAND_ARRAY              "bands_array"
#define LENS_ARRAY              "lens_array"

#define BAND_START_STR      "band_start"
#define BAND_START_VAL      "300"
#define BAND_END_STR        "band_end"
#define BAND_END_VAL        "830"
#define INITIAL_BAND_STR    "initial_band"
#define INITIAL_BAND_VAL    "560"
#define BAND_STEP_STR       "band_step"
#define BAND_STEP_VAL       "5"

#define FIXED_NAME1_STR     "fixed_name1"
#define FIXED_NAME1_VAL     ""
#define FIXED_NAME2_STR     "fixed_name2"
#define FIXED_NAME2_VAL     ""
#define FIXED_NAME3_STR     "fixed_name3"
#define FIXED_NAME3_VAL     ""
#define FIXED_BAND1_STR     "fixed_band1"
#define FIXED_BAND1_VAL     "0"
#define FIXED_BAND2_STR     "fixed_band2"
#define FIXED_BAND2_VAL     "0"
#define FIXED_BAND3_STR     "fixed_band3"
#define FIXED_BAND3_VAL     "0"

/*!
 * Structure used in the Settings Hash table. Contains the ID String and Default Value String for Setting
 */
typedef struct {
    QString id_str;             /*!< Settings ID String, this is the value written to the underlying settings file      */
    QString default_val;         /*!< Setting default Value, this is the default value used if the setting is not found  */
}setting_def_t;

class TucSettings
{
public:
    TucSettings();
    ~TucSettings();

    QString readSetting(setting_ids id);
    void writeSetting(setting_ids id, QString value);

private:
    void readInitFile();
    void setDefaultSettings(void);

    QSettings *mSettings;
    QHash<setting_ids, setting_def_t>mSettingId;
};

#endif // TUCSETTINGS_H
