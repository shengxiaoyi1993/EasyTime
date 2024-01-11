#ifndef EASYTIME_H
#define EASYTIME_H

/*
* Copyright (c) 2022-06-01, DigitalVersion Inc. - All Rights Reserved
*
********************************************************************************
*
* @Author: shengxiaoyi2014@icloud.com (XiaoYi Sheng)
*
********************************************************************************
*
* @Usage:
* - 该模块用于复用时间相关的接口
*
********************************************************************************
* @note
* 基准时间: 时刻UTC 1970-01-01 00:00:00.000
*
*/

#include <string>
#include <chrono>
#include <set>
#include <vector>
#include <macro_glog/macro_glog.h>

namespace ns_easytime {

/**
 * @brief getTimePointString_ns
 * - 获取纳秒精度的时间点的字符串
 * @param v_time
 * - 距离基准时间的纳秒数
 * @return
 */
std::string getTimePointString_ns(long long v_time);

std::string getTimePointString(long long v_time);

/**
 * @brief getTimeDurationString_ns
 * @param v_time
 * @return
 */
// std::string getTimeDurationString_ns(long long v_time);

long long getTimeCode_ms();

///
/// T can be std::chrono::milliseconds or others
///
template <typename T>
inline long long getTimeCode() {
  return std::chrono::duration_cast<T>(
      std::chrono::system_clock::now().time_since_epoch()).count();
}

/// 最小公倍数
int lcm(int a, int b);
template <typename T>
bool isEqual(const std::set<T>& set1, const std::set<T>& set2) {
  if (set1.size() != set2.size()) {
    return false;
  }
  for (T i : set1) {
    if (set2.find(i) == set2.end()) {
      return false;
    }
  }
  return true;
}
///
/// T 要求从0逐1增加
/// 时段也可以没有状态
/// 在求

template <typename T>
class Segment {
 public:
  long long __start;
  long long __duration;

  std::set<T> __set_state;

 public:
  /// 按照提供的有序数组进行分割,若分割点在区间外,则不处理
  std::vector<Segment<T>> split(const std::vector<long long>& vi_list) const;

  /// 合并接壤的区间
  /// - 区间边界可以通过"=="进行判断
  /// - vi_list 各区间可能接壤,但是不会重叠
  static void mergeSegmet(std::vector<Segment<T>>& vo_list,
                          const std::vector<Segment<T>>& vi_list);
};

template <typename T>
std::vector<Segment<T>> Segment<T>::split(const std::vector<long long>& vi_list)
    const {
  /// 要求满足vi_list在Segment的区间范围内
  std::vector<Segment<T>> list_result;

  long long pos_start = __start;
  for (int var = 0; var < vi_list.size(); ++var) {
    if (vi_list[var] > __start && vi_list[var] < __start + __duration) {
      Segment<T> item;
      item.__set_state = __set_state;
      item.__start = pos_start;
      item.__duration = vi_list[var] - pos_start;
      pos_start = vi_list[var];
      list_result.push_back(std::move(item));
    }
  }

  if (pos_start < __start + __duration) {
    Segment<T> item;
    item.__set_state = __set_state;
    item.__start = pos_start;
    item.__duration = __start + __duration - pos_start;
    pos_start = __start + __duration;
    list_result.push_back(std::move(item));
  }
  return list_result;
}

template <typename T>
void Segment<T>::mergeSegmet(std::vector<Segment<T>>& vo_list,
                             const std::vector<Segment<T>>& vi_list) {

  /// 合并接壤的区间
  /// - 逐个与结果列表中的前一项进行比较
  ///  - 若结果列表为空,直接添加
  ///  - 若接壤且状态相同,合并到上一个区间
  ///  - 若不接壤,添加为新的项
  //  vo_list
  for (int var = 0; var < vi_list.size(); ++var) {
    if (vo_list.empty()) {
      vo_list.push_back(vi_list[var]);
    } else {
      if ((vo_list.rbegin()->__duration + vo_list.rbegin()->__start ==
           vi_list[var].__start) &&
          isEqual(vo_list.rbegin()->__set_state, vi_list[var].__set_state)) {
        /// 接壤,合并
        vo_list.rbegin()->__duration += vi_list[var].__duration;
      } else {
        vo_list.push_back(vi_list[var]);
      }
    }
  }
}

template <typename T>
class Periodicity {

 public:
  long long __length;
  /// 要求__list_segment中的各个时段
  /// - 在__length范围内
  /// - 没有重叠
  /// - 按照先后顺序进行排序
  /// - 各时段不能为0
  std::vector<Segment<T>> __list_segment;

  /// null 与 任意集合T的交集为T
  /// null 与 任意集合T厄并集为null
  /// 表示不合理或用户指定无效的情况
  bool __is_null;

 public:
  Periodicity();

  void getSplitSeg(const Periodicity<T>& vi_pd, Periodicity<T>& vo_result,
                   std::vector<Segment<T>>& vo_list_this,
                   std::vector<Segment<T>>& vo_list_pd) const;

  //  Periodicity intersectionOp(const Periodicity& vi_pd) const;
  Periodicity<T> unionOp(const Periodicity<T>& vi_pd) const;

  /// 拓展两个周期为最大公倍数
  /// 对于vi_pd中有状态的时段,总是优先采用
  Periodicity<T> maskOp(const Periodicity<T>& vi_pd) const;

  //  /// 交集
  //  static Periodicity intersectionOp(const std::vector<Periodicity>&
  // vi_list);
  /// 并集
  static Periodicity<T> unionOp(const std::vector<Periodicity<T>>& vi_list);

  bool isNull() const;
};

template <typename T>
bool Periodicity<T>::isNull() const {
  return __is_null;
}

template <typename T>
Periodicity<T>::Periodicity()
    : __length(0), __is_null(true) {}

template <typename T>
void Periodicity<T>::getSplitSeg(const Periodicity<T>& vi_pd,
                                 Periodicity<T>& vo_result,
                                 std::vector<Segment<T>>& vo_list_this,
                                 std::vector<Segment<T>>& vo_list_pd) const {

  /// 求并集
  /// 结果集合的周期为两周期的最大公倍数
  /// 先将两集合拓展,然后将相同状态的合并.
  /// ? 如何处理两重叠的时间内有不同的状态,此时要求状态某个时段具有多个状态
  /// 求交集时各个状态需要一一对应
  //  Periodicity<T> result;
  vo_result.__length = lcm(vi_pd.__length, __length);

  MACRO_GLOG_INFO("vo_result.__length:" << vo_result.__length);

  /// 拓展this
  Periodicity<T> thisextend;
  {
    thisextend.__length = vo_result.__length;
    thisextend.__is_null = true;
    int num = thisextend.__length / __length;
    for (int var = 0; var < num; ++var) {
      for (int nt = 0; nt < __list_segment.size(); ++nt) {
        Segment<T> item;
        item.__start = __length * var + __list_segment[nt].__start;
        item.__duration = __list_segment[nt].__duration;
        item.__set_state = __list_segment[nt].__set_state;
        thisextend.__list_segment.push_back(std::move(item));
      }
    }
  }

  MACRO_GLOG_INFO(
      "thisextend.__list_segment.size:" << thisextend.__list_segment.size());
  {
    std::string printstring;
    for (int var = 0; var < thisextend.__list_segment.size(); ++var) {
      printstring +=
          "(" + std::to_string(thisextend.__list_segment[var].__start) + "," +
          std::to_string(thisextend.__list_segment[var].__duration) + ")";
    }
    MACRO_GLOG_INFO("__list_segment:" << printstring);
  }

  /// 拓展vi_pd
  Periodicity<T> vi_pdextend;
  {
    vi_pdextend.__length = vo_result.__length;
    vi_pdextend.__is_null = true;
    int num = vi_pdextend.__length / vi_pd.__length;
    for (int var = 0; var < num; ++var) {
      for (int nt = 0; nt < vi_pd.__list_segment.size(); ++nt) {
        Segment<T> item;
        item.__start = vi_pd.__length * var + vi_pd.__list_segment[nt].__start;
        item.__duration = vi_pd.__list_segment[nt].__duration;
        item.__set_state = vi_pd.__list_segment[nt].__set_state;
        vi_pdextend.__list_segment.push_back(std::move(item));
      }
    }
  }
  MACRO_GLOG_INFO(
      "vi_pdextend.__list_segment.size:" << vi_pdextend.__list_segment.size());

  /// 找出周期的时间点
  /// 每各周期进行将时段进行分割,使两个周期具有相同的分割
  /// 然后逐个比较各个时段的状态,若具有相同的状态,则将该时段添加到结果周期中
  /// 然后再整理结果周期中的时段,接壤的相同状态的时段合并为一个
  /// 输出结果

  /// 所有的断点
  std::set<long long> set_point;
  for (int var = 0; var < thisextend.__list_segment.size(); ++var) {
    set_point.insert(thisextend.__list_segment[var].__start);
    set_point.insert(thisextend.__list_segment[var].__start +
                     thisextend.__list_segment[var].__duration);
  }

  for (int var = 0; var < vi_pdextend.__list_segment.size(); ++var) {
    set_point.insert(vi_pdextend.__list_segment[var].__start);
    set_point.insert(vi_pdextend.__list_segment[var].__start +
                     vi_pdextend.__list_segment[var].__duration);
  }

  /// 转换成vector
  std::vector<long long> list_point(set_point.begin(), set_point.end());
  MACRO_GLOG_INFO("list_point.size:" << list_point.size());
  {
    std::string printstring;
    for (int var = 0; var < list_point.size(); ++var) {
      printstring += std::to_string(list_point[var]) + ",";
    }
    MACRO_GLOG_INFO("list_point:" << printstring);
  }

  /// if thisextend.__list_segment 中的一项包含
  /// list_point中的数字,且不是边界,则进行分割
  std::vector<std::vector<long long>> list_split_this_lseg(
      thisextend.__list_segment.size());
  {
    int index_point = 0;
    for (int var = 0; var < thisextend.__list_segment.size(); ++var) {
      for (; index_point < list_point.size(); ++index_point) {

        /// 当其值大于thisextend.__list_segment有边界时断开
        if (list_point[index_point] ==
            thisextend.__list_segment[var].__start +
                thisextend.__list_segment[var].__duration) {
          break;
        } else if (list_point[index_point] >
                   thisextend.__list_segment[var].__start +
                       thisextend.__list_segment[var].__duration) {
          if (index_point > 0) {
            index_point--;
          }
          break;
        } else {
          if (list_point[index_point] >
              thisextend.__list_segment[var].__start) {
            /// 此时在区间内,且不为边界
            list_split_this_lseg[var].push_back(list_point[index_point]);
          }
        }
      }
    }
  }

  /// 得到区间细分后的列表
  for (int var = 0; var < thisextend.__list_segment.size(); ++var) {
    if (list_split_this_lseg[var].size() > 0) {
      /// 将Segment以某些数值,划分成若干段
      std::vector<Segment<T>> list_tmp =
          thisextend.__list_segment[var].split(list_split_this_lseg[var]);
      vo_list_this.insert(vo_list_this.end(), list_tmp.begin(), list_tmp.end());

    } else {
      vo_list_this.push_back(thisextend.__list_segment[var]);
    }
  }

  {
    std::string printstring;
    for (int var = 0; var < vo_list_this.size(); ++var) {
      printstring += "(" + std::to_string(vo_list_this[var].__start) + "," +
                     std::to_string(vo_list_this[var].__duration) + ")";
    }
    MACRO_GLOG_INFO("vo_list_this:" << printstring);
  }

  std::vector<std::vector<long long>> list_split_vi_pd_lseg(
      vi_pdextend.__list_segment.size());
  {
    int index_point = 0;
    for (int var = 0; var < vi_pdextend.__list_segment.size(); ++var) {
      for (; index_point < list_point.size(); ++index_point) {

        /// 当其值大于vi_pdextend.__list_segment有边界时断开
        if (list_point[index_point] ==
            vi_pdextend.__list_segment[var].__start +
                vi_pdextend.__list_segment[var].__duration) {
          break;
        } else if (list_point[index_point] >
                   vi_pdextend.__list_segment[var].__start +
                       vi_pdextend.__list_segment[var].__duration) {
          if (index_point > 0) {
            index_point--;
          }
          break;
        } else {
          if (list_point[index_point] >
              vi_pdextend.__list_segment[var].__start) {
            /// 此时在区间内,且不为边界
            list_split_vi_pd_lseg[var].push_back(list_point[index_point]);
          }
        }
      }
    }
  }

  /// 得到区间细分后的列表
  for (int var = 0; var < vi_pdextend.__list_segment.size(); ++var) {
    if (list_split_vi_pd_lseg[var].size() > 0) {
      MACRO_GLOG_INFO(var << " list_split_vi_pd_lseg[var].size:"
                          << list_split_vi_pd_lseg[var].size());
      /// 将Segment以某些数值,划分成若干段
      std::vector<Segment<T>> list_tmp =
          vi_pdextend.__list_segment[var].split(list_split_vi_pd_lseg[var]);

      MACRO_GLOG_INFO(var << " list_tmp.size:" << list_tmp.size());

      vo_list_pd.insert(vo_list_pd.end(), list_tmp.begin(), list_tmp.end());

    } else {
      vo_list_pd.push_back(vi_pdextend.__list_segment[var]);
    }
  }
}

template <typename T>
Periodicity<T> Periodicity<T>::unionOp(const Periodicity<T>& vi_pd) const {
  if (isNull() || vi_pd.isNull()) {
    return Periodicity<T>();
  }

  std::vector<Segment<T>> list_seg_detail_vi_pd;
  std::vector<Segment<T>> list_seg_detail_this;
  Periodicity<T> result;

  getSplitSeg(vi_pd, result, list_seg_detail_this, list_seg_detail_vi_pd);

  /// 若前面的计算 步骤正常 list_seg_detail_vi_pd 与list_seg_detail_this
  /// 包含的元素相同

  if (list_seg_detail_vi_pd.size() != list_seg_detail_this.size()) {
    throw "list_seg_detail_vi_pd.size() != list_seg_detail_this.size()";
  }

  /// thisextend.__list_segment 中的每个区间
  /// 都以list_split_this_lseg中的数值进行分裂
  ///

  /// 细分后的区间进行比较状态是否相同
  /// 提取相同状态的区间组成新的列表
  std::vector<Segment<T>> list_same_state;
  for (int var = 0; var < list_seg_detail_vi_pd.size(); ++var) {
    if (isEqual(list_seg_detail_vi_pd[var].__set_state,
                list_seg_detail_this[var].__set_state)) {
      list_same_state.push_back(list_seg_detail_this[var]);
    }
  }

  Segment<T>::mergeSegmet(result.__list_segment, list_same_state);

  result.__is_null = false;
  return result;
}

template <typename T>
Periodicity<T> Periodicity<T>::maskOp(const Periodicity<T>& vi_pd) const {
  std::vector<Segment<T>> list_seg_detail_vi_pd;
  std::vector<Segment<T>> list_seg_detail_this;
  Periodicity<T> result;
  getSplitSeg(vi_pd, result, list_seg_detail_this, list_seg_detail_vi_pd);

  if (list_seg_detail_vi_pd.size() != list_seg_detail_this.size()) {
    throw "list_seg_detail_vi_pd.size() != list_seg_detail_this.size()";
  }

  /// 对于进行对应细分的周期
  /// 每个时段进行比较
  /// - 若vi_pd无状态,采用this
  /// - 若vi_pd有状态,采用vi_pd

  std::vector<Segment<T>> list_seg_result(list_seg_detail_vi_pd.size());
  for (int var = 0; var < list_seg_detail_vi_pd.size(); ++var) {
    if (list_seg_detail_vi_pd[var].__set_state.empty()) {
      list_seg_result[var] = list_seg_detail_this[var];
    } else {
      list_seg_result[var] = list_seg_detail_vi_pd[var];
    }
  }
  Segment<T>::mergeSegmet(result.__list_segment, list_seg_result);

  result.__is_null = false;
  return result;
}

// template <typename T>
// Periodicity Periodicity::intersectionOp(
//    const std::vector<Periodicity>& vi_list) {}
}

#endif
