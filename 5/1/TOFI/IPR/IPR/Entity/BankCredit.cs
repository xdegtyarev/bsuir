using System;
using MongoDB.Bson.Serialization.Attributes;
using MongoDB.Bson.Serialization.IdGenerators;

namespace IPR.Entity
{
    public enum BankCreditStatus{
        DraftRequest,
        PendingRequest,
        Approved,
        Rejected,
        Active,
        Healthy,
        Overdue,
        Repaid
    }

    public class BankCredit
    {
        [BsonId(IdGenerator = typeof(StringObjectIdGenerator))]
        public string ID { get; set; }

        [BsonElement("status")]
        public BankCreditStatus status{ get; set; }

        public string BankClientID{ get; set; }
        public string BankEmployeeID{ get; set; }

        public int currency{ get; set; }
        public double creditAmount{ get; set; }
        public int creditTerm{ get; set; }

        public double creditToPayInCurrentPeriod{ get; set; } //prop
        public double totalOverpay{ get; set; }
        public double rate{ get; set; }
        public double effectiveRate{ get; set; }
		
        public BankCredit()
        {
            status = BankCreditStatus.PendingRequest;
        }
    }
}
