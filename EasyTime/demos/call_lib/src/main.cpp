#include <iostream>
#include <chrono>
#include <EasyTime/EasyTime.h>
#include <QDateTime>
#include <QLocale>

void demo_day_week_month_starttime();

/// Give two Periodicity with different length, and status segment
/// get the segment of two
void demo_Periodicity_union();

void demo_Periodicity_mask();

int main(int argc, char const *argv[]) {
  demo_Periodicity_mask();

  //  demo_day_week_month_starttime();
  //  long long
  // time_now=std::chrono::system_clock::now().time_since_epoch().count();
  //  std::string str_time_now=ns_easytime::getTimePointString_ns(time_now);
  //  std::cout<<"str_time_now:"<<str_time_now<<std::endl;

  //  long long ms=ns_easytime::getTimeCode_ms();

  //  std::cout<<"ms:"<<ms<<std::endl;

  return 0;
}

enum ENUM_STATUS {
  ENUM_STATUS_ON = 0,
  ENUM_STATUS_OFF,
  ENUM_STATUS_QUANTITY
};

void demo_Periodicity_mask() {

  /// 指定两个周期内的状态时段

  /// 每小时的第5分钟保持启动,其他时间保持关闭

  ns_easytime::Periodicity<ENUM_STATUS> period_control;

  /// off 4 minutes
  {
    ns_easytime::Segment<ENUM_STATUS> segment_tmp;
    segment_tmp.__start = 0;
    segment_tmp.__duration = 60 * 4;
    segment_tmp.__set_state.insert(ENUM_STATUS_OFF);
    period_control.__list_segment.push_back(std::move(segment_tmp));
  }
  /// on 1 minutes
  {
    ns_easytime::Segment<ENUM_STATUS> segment_tmp;
    segment_tmp.__start = 60 * 4;
    segment_tmp.__duration = 60;
    segment_tmp.__set_state.insert(ENUM_STATUS_ON);
    period_control.__list_segment.push_back(std::move(segment_tmp));
  }
  /// off 55 minutes
  {
    ns_easytime::Segment<ENUM_STATUS> segment_tmp;
    segment_tmp.__start = 60 * 5;
    segment_tmp.__duration = 60 * 60 - 60 * 5;
    segment_tmp.__set_state.insert(ENUM_STATUS_OFF);
    period_control.__list_segment.push_back(std::move(segment_tmp));
  }

  period_control.__is_null = false;
  period_control.__length = 1 * 60 * 60;

  /// 一天中的12:30 至13:30 保持关闭,其他时间没有状态
  ns_easytime::Periodicity<ENUM_STATUS> period_daily;

  {
    ns_easytime::Segment<ENUM_STATUS> segment_tmp;
    segment_tmp.__start = 0;
    segment_tmp.__duration = 12 * 60 * 60 + 30 * 60;
    //    segment_tmp.__set_state.insert(ENUM_STATUS_OFF);
    //      segment_tmp.__set_state.insert(ENUM_STATUS_ON);
    period_daily.__list_segment.push_back(std::move(segment_tmp));
  }

  {
    ns_easytime::Segment<ENUM_STATUS> segment_tmp;
    segment_tmp.__start = 12 * 60 * 60 + 30 * 60;
    segment_tmp.__duration = 1 * 60 * 60;
    segment_tmp.__set_state.insert(ENUM_STATUS_OFF);
    period_daily.__list_segment.push_back(std::move(segment_tmp));
  }

  {
    ns_easytime::Segment<ENUM_STATUS> segment_tmp;
    segment_tmp.__start = 13 * 60 * 60 + 30 * 60;
    segment_tmp.__duration = 10 * 60 * 60 + 30 * 60;
    //      segment_tmp.__set_state.insert(ENUM_STATUS_ON);
    period_daily.__list_segment.push_back(std::move(segment_tmp));
  }

  period_daily.__is_null = false;
  period_daily.__length = 24 * 60 * 60;

  ns_easytime::Periodicity<ENUM_STATUS> result =
      period_control.maskOp(period_daily);

  std::cout << "result.isNull:" << result.isNull() << std::endl;
  std::cout << "result.__length:" << result.__length << std::endl;
  std::cout << "result.__list_segment:" << result.__list_segment.size()
            << std::endl;
  for (int var = 0; var < result.__list_segment.size(); ++var) {

    std::cout << QTime::fromMSecsSinceStartOfDay(
                     result.__list_segment[var].__start * 1000)
                     .toString()
                     .toStdString() << std::endl;
    std::cout << "__start:" << result.__list_segment[var].__start << std::endl;
    std::cout << "__duration:" << result.__list_segment[var].__duration
              << std::endl;
    std::cout << "__set_state:";
    for (const auto &it : result.__list_segment[var].__set_state) {
      std::cout << it << " ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

void demo_Periodicity_union() {
  /// 指定两个周期内的状态时段

  /// 每小时的第5分钟保持启动,其他时间保持关闭

  ns_easytime::Periodicity<ENUM_STATUS> period_control;

  /// off 4 minutes
  {
    ns_easytime::Segment<ENUM_STATUS> segment_tmp;
    segment_tmp.__start = 0;
    segment_tmp.__duration = 60 * 4;
    segment_tmp.__set_state.insert(ENUM_STATUS_OFF);
    period_control.__list_segment.push_back(std::move(segment_tmp));
  }
  /// on 1 minutes
  {
    ns_easytime::Segment<ENUM_STATUS> segment_tmp;
    segment_tmp.__start = 60 * 4;
    segment_tmp.__duration = 60;
    segment_tmp.__set_state.insert(ENUM_STATUS_ON);
    period_control.__list_segment.push_back(std::move(segment_tmp));
  }
  /// off 55 minutes
  {
    ns_easytime::Segment<ENUM_STATUS> segment_tmp;
    segment_tmp.__start = 60 * 5;
    segment_tmp.__duration = 60 * 60 - 60 * 5;
    segment_tmp.__set_state.insert(ENUM_STATUS_OFF);
    period_control.__list_segment.push_back(std::move(segment_tmp));
  }

  period_control.__is_null = false;
  period_control.__length = 1 * 60 * 60;

  /// 一天中的12:30 至13:30 保持关闭,其他时间没有状态
  ns_easytime::Periodicity<ENUM_STATUS> period_daily;

  {
    ns_easytime::Segment<ENUM_STATUS> segment_tmp;
    segment_tmp.__start = 0;
    segment_tmp.__duration = 12 * 60 * 60 + 30 * 60;
    //    segment_tmp.__set_state.insert(ENUM_STATUS_OFF);
    segment_tmp.__set_state.insert(ENUM_STATUS_ON);
    period_daily.__list_segment.push_back(std::move(segment_tmp));
  }

  {
    ns_easytime::Segment<ENUM_STATUS> segment_tmp;
    segment_tmp.__start = 12 * 60 * 60 + 30 * 60;
    segment_tmp.__duration = 1 * 60 * 60;
    segment_tmp.__set_state.insert(ENUM_STATUS_OFF);
    period_daily.__list_segment.push_back(std::move(segment_tmp));
  }

  {
    ns_easytime::Segment<ENUM_STATUS> segment_tmp;
    segment_tmp.__start = 13 * 60 * 60 + 30 * 60;
    segment_tmp.__duration = 10 * 60 * 60 + 30 * 60;
    segment_tmp.__set_state.insert(ENUM_STATUS_ON);
    period_daily.__list_segment.push_back(std::move(segment_tmp));
  }

  period_daily.__is_null = false;
  period_daily.__length = 24 * 60 * 60;
  //  std::vector<ns_easytime::Periodicity<ENUM_STATUS>> list;
  //  list.push_back(period_daily);
  //  list.push_back(period_control);

  //  ns_easytime::Periodicity<ENUM_STATUS> result =
  //      period_control.unionOp(period_daily);

  ns_easytime::Periodicity<ENUM_STATUS> result =
      period_control.unionOp(period_daily);

  std::cout << "result.isNull:" << result.isNull() << std::endl;
  std::cout << "result.__length:" << result.__length << std::endl;
  std::cout << "result.__list_segment:" << result.__list_segment.size()
            << std::endl;
  for (int var = 0; var < result.__list_segment.size(); ++var) {

    std::cout << QTime::fromMSecsSinceStartOfDay(
                     result.__list_segment[var].__start * 1000)
                     .toString()
                     .toStdString() << std::endl;
    std::cout << "__start:" << result.__list_segment[var].__start << std::endl;
    std::cout << "__duration:" << result.__list_segment[var].__duration
              << std::endl;
    std::cout << "__set_state:";
    for (const auto &it : result.__list_segment[var].__set_state) {
      std::cout << it << " ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;

  /// 我们希望得到的结果是无状态的采用有状态的
  /// 关闭和开启冲突的采用,关闭状态
  ///
}

void demo_day_week_month_starttime() {
  QLocale lc(QLocale::English);

  QDateTime dt = QDateTime::currentDateTime();
  uint tc_dt = QDateTime::currentDateTime().toTime_t();
  std::cout << "tc_dt:" << tc_dt << std::endl;
  QString dt_string = lc.toString(dt, "yyyy.MM.ddd.dd");
  std::cout << "dt:" << dt_string.toStdString() << std::endl;

  /// print day of week
  {
    std::vector<QString> list_weekday(7);
    for (int var = 0; var < 7; ++var) {
      list_weekday[var] = dt.addDays(var).toString("dddd");
      std::cout << var << " " << list_weekday[var].toStdString() << std::endl;
      std::cout << var << " " << lc.toString(dt.addDays(var), "dddd")
                                     .toStdString() << std::endl;
      std::cout << var << " " << lc.toString(dt.addDays(var), "ddd")
                                     .toStdString() << std::endl;
    }
  }

  auto list = dt_string.split('.');

  QString dt_string_day = list[0] + "." + list[1] + "." + list[3];
  QDateTime dt_day = QDateTime::fromString(dt_string_day, "yyyy.MM.dd");

  std::cout << "dt_string_day:" << dt_string_day.toStdString() << std::endl;
  std::cout << "dt_day:" << dt_day.toString().toStdString() << std::endl;

  std::vector<QString> list_weekday{"Mon", "Tue", "Wed", "Thu",
                                    "Fri", "Sat", "Sun"};

  int index = -1;
  for (int var = 0; var < list_weekday.size(); ++var) {
    if (list_weekday[var] == list[2]) {
      index = var;
      break;
    }
  }

  if (index == -1) {
    return;
  }

  QString dt_string_week = list[0] + "." + list[1] + "." + list[3];
  //  QDateTime dt_week=QDateTime::fromString(dt_string_week,"yyyy.MM.dd");
  QDateTime dt_week =
      lc.toDateTime(dt_string_week, "yyyy.MM.dd").addDays(-index);
  std::cout << "dt_string_week:" << dt_string_week.toStdString() << std::endl;
  std::cout << "dt_week:" << dt_week.toString().toStdString() << std::endl;

  QString dt_string_month = list[0] + "." + list[1];
  QDateTime dt_month = QDateTime::fromString(dt_string_month, "yyyy.MM");

  std::cout << "dt_string_month:" << dt_string_month.toStdString() << std::endl;
  std::cout << "dt_month:" << dt_month.toString().toStdString() << std::endl;
}
